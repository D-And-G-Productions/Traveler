# Traveler

## Repository layout

- `traveler_web/`
  - Client application. Talks to `backend_api` via HTTP only.
- `backend_api/`
  - Crow C++ service. Owns HTTP API, auth, validation, and application logic.
  - Talks to Postgres through repository interfaces implemented by the Postgres adapter.
- `database/`
  - Database service definition and migrations.
  - Owns schema evolution (up and down migrations), seed scripts, and local dev DB config.
- `documentation/`
  - Architecture notes and diagrams.

## Service boundaries

### traveler_web
- Knows: HTTP contract (request and response shapes).
- Does not know: SQL, schema, database credentials, libpqxx, Postgres.

### backend_api
- Knows: domain rules, use-cases, and repository interfaces.
- Does not know: database schema details in the web or core layers.
- Rule: only the Postgres adapter knows SQL.

### database
- Knows: schema, migrations, extensions, indexes.
- Does not know: C++ code, DTOs, routing, web concerns.

## Backend layering rules

Inside `backend_api/src`:

- `web/`
  - HTTP routes/controllers/middleware/DTOs.
  - Converts HTTP DTOs to domain inputs and converts domain outputs back to DTOs.
  - Must not contain SQL, and must not include libpqxx headers.

- `core/`
  - Domain and application logic.
  - Exposes repository interfaces (ports) and a UnitOfWork abstraction.
  - Must not include libpqxx headers.
  - Must not depend on web DTOs.

- `persistence_postgres/`
  - Implements core repository interfaces using libpqxx.
  - Owns SQL queries, mapping, transaction implementation.

libpqxx usage note: SQL work is done inside transaction objects (for example `pqxx::work tx(conn)`), not directly on the connection. See libpqxx getting-started and overview docs. :contentReference[oaicite:0]{index=0}

## Local development

### Prerequisites
- Docker + Docker Compose

### Environment variables
Create a `.env` based on `.env.example`. For Postgres container configuration, follow the official Postgres image environment variables (`POSTGRES_USER`, `POSTGRES_PASSWORD`, `POSTGRES_DB`). :contentReference[oaicite:1]{index=1}

### Run database only
From repo root:

- `docker compose -f database/compose.db.yaml up -d`

### Run migrations
Migrations are stored in `database/migrations` and must have an up and down file per version.

We use `golang-migrate/migrate` as the migration runner (CLI). :contentReference[oaicite:2]{index=2}

Recommended pattern:
- Create migration:
  - `migrate create -ext sql -dir database/migrations -seq <name>`
- Apply:
  - `migrate -path database/migrations -database "<POSTGRES_DSN>" up`
- Rollback:
  - `migrate -path database/migrations -database "<POSTGRES_DSN>" down 1`

## Rules of change (do this every time)

These rules exist to prevent the database and web server from drifting into tight coupling.

### 1. Schema changes always go through migrations
- Do not edit schema manually.
- Every schema change must ship as:
  - `database/migrations/<version>_<name>.up.sql`
  - `database/migrations/<version>_<name>.down.sql`
- The down migration must be real. If the change is not safely reversible, document why in the migration file header and provide a safe operational rollback plan.

### 2. Prefer backwards-compatible database changes
Use an expand then contract approach:

Expand phase (compatible):
- Add new tables or columns as nullable.
- Add new indexes concurrently where applicable.
- Keep old columns and behaviours working.

Contract phase (later PR):
- Backfill data if needed.
- Flip constraints (NOT NULL) only after code has been deployed and data is consistent.
- Remove deprecated columns only after at least one release cycle.

Rule: do not merge a PR that requires deploying DB and API at the exact same moment.

### 3. No SQL outside the Postgres adapter
- SQL strings live in `backend_api/src/persistence_postgres/**`.
- Web controllers must call core use-cases.
- Core use-cases must call repository interfaces, not concrete Postgres classes.

### 4. No libpqxx outside the Postgres adapter
- Only files under `backend_api/src/persistence_postgres/**` may include `<pqxx/pqxx>`.
- Core and web code must compile without linking libpqxx.

### 5. Repository interfaces are the contract between core and persistence
When changing persistence behaviour:
- First change the domain model and repository interface in `core/ports`.
- Then update `persistence_postgres/` to implement the new interface.
- Do not pass web DTOs into repository methods.

### 6. Transactions are owned by the application boundary
- Begin a UnitOfWork at the start of a request (or use-case).
- Commit on success; rollback on error.
- Repository methods are transaction-agnostic and operate within the active UnitOfWork.

libpqxx reminder: do SQL work inside a transaction object and commit explicitly when needed. :contentReference[oaicite:3]{index=3}

### 7. Parameterise queries, do not build SQL with string concatenation
- Use prepared statements or parameterised queries (`$1`, `$2`, etc).
- This is required for correctness and safety. libpqxx documents parameterised execution and prepared statements. :contentReference[oaicite:4]{index=4}

### 8. Testing requirements for persistence changes
Any PR that changes:
- domain model, repository interfaces, or migrations

Must include:
- updated migrations (up and down)
- updated Postgres adapter code
- integration tests under `backend_api/tests/integration/persistence_postgres/` that:
  - start with an empty DB
  - apply migrations
  - execute the repository methods exercised by the change

### 9. Operational rule: migrations are run as a dedicated step
- Do not run migrations automatically on API startup in production.
- Migrations should run as a separate job in CI/CD before rolling out the new API container.
- This keeps failure modes clear and prevents partial rollouts.

## Feature implementation checklist

When a feature needs new persistence:
1. Add a migration (up and down) in `database/migrations`.
2. Update core domain and repository interface in `backend_api/src/core/ports`.
3. Implement repository changes in `backend_api/src/persistence_postgres`.
4. Add or update integration tests for the repository.
5. If the HTTP API changes, update DTOs in `backend_api/src/web/dto` and the client in `traveler_web`.
