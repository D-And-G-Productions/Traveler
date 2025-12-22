import { StyleSheet } from 'react-native';

const styles = StyleSheet.create({
    centerContainer: { flex: 1, justifyContent: 'center', alignItems: 'center' },
    container: { flex: 1, padding: 20, maxWidth: 500, alignSelf: 'center', width: '100%' },
    card: { width: 300, padding: 20, backgroundColor: 'white', borderRadius: 8, elevation: 3 },
    title: { fontSize: 20, marginBottom: 20, textAlign: 'center' },
    input: { height: 40, borderBottomWidth: 1, marginBottom: 20 },
    header: { flexDirection: 'row', justifyContent: 'space-between', marginBottom: 20 },
    webPicker: { padding: 10, width: '100%', marginBottom: 20 }
});

export default styles;

