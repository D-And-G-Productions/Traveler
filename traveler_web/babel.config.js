module.exports = function (api) {
    api.cache(true);
    return {
        presets: ['babel-preset-expo'], // CHANGE THIS LINE
        plugins: [
            [
                'module:react-native-dotenv',
                {
                    moduleName: '@env',
                    path: '.env',
                },
            ],
        ],
    };
};
