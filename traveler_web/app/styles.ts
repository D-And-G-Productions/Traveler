import { StyleSheet } from 'react-native';

export const GlobalStyles = StyleSheet.create({
    // Containers
    centerContainer: {
        flex: 1,
        justifyContent: 'center',
        alignItems: 'center',
        backgroundColor: '#f8f9fa',
    },
    container: {
        flex: 1,
        padding: 20,
        maxWidth: 500,
        alignSelf: 'center',
        width: '100%',
    },

    // Cards & Layouts
    card: {
        width: 320,
        padding: 25,
        backgroundColor: 'white',
        borderRadius: 12,
        shadowColor: '#000',
        shadowOffset: { width: 0, height: 2 },
        shadowOpacity: 0.1,
        shadowRadius: 4,
        elevation: 5,
    },
    header: {
        flexDirection: 'row',
        justifyContent: 'space-between',
        alignItems: 'center',
        marginBottom: 30,
    },

    // Typography
    title: {
        fontSize: 22,
        fontWeight: 'bold',
        marginBottom: 20,
        textAlign: 'center',
        color: '#333',
    },
    label: {
        fontSize: 14,
        fontWeight: '600',
        marginBottom: 5,
        color: '#666',
    },

    // Inputs
    input: {
        height: 45,
        borderBottomWidth: 1,
        borderBottomColor: '#ddd',
        marginBottom: 20,
        paddingHorizontal: 8,
        fontSize: 16,
    },
    webPicker: {
        padding: 12,
        width: '100%',
        marginBottom: 25,
        borderRadius: 6,
        fontSize: 16,
        fontFamily: 'sans-serif',
    },
});

