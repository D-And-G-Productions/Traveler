import { StyleSheet } from 'react-native';

const COLORS = {
    primary: '#2A2A2A',   // Almost black for strong contrast
    background: '#F5F7FA', // Very light cool gray
    inputBg: '#E8F0FE',    // Slight contrast for inputs
    text: '#1A1A1A',
    subText: '#8E8E93',
    white: '#FFFFFF'
};

export const GlobalStyles = StyleSheet.create({
    // Containers
    centerContainer: {
        flex: 1,
        justifyContent: 'center',
        alignItems: 'center',
        backgroundColor: COLORS.background,
        padding: 20,
    },
    // Cards
    card: {
        width: '100%',
        maxWidth: 400, // Slightly wider for modern feel
        padding: 30,
        backgroundColor: COLORS.white,
        borderRadius: 20, // Softer corners
        // Soft, diffuse shadow
        shadowColor: '#000',
        shadowOffset: { width: 0, height: 10 },
        shadowOpacity: 0.05,
        shadowRadius: 15,
        elevation: 2,
    },

    // Typography
    title: {
        fontSize: 28,
        fontWeight: '700',
        marginBottom: 30,
        textAlign: 'center',
        color: COLORS.primary,
        letterSpacing: 0.5,
    },

    label: {
        fontSize: 14,
        fontWeight: '600',
        marginBottom: 8,
        color: COLORS.text,
    },

    // Inputs (Modern Filled Style)
    input: {
        height: 55,
        backgroundColor: '#F7F7F7', // Light gray fill
        borderRadius: 12,
        marginBottom: 15,
        paddingHorizontal: 15,
        fontSize: 16,
        color: COLORS.text,
    },

    // Buttons
    primaryButton: {
        backgroundColor: COLORS.primary,
        height: 55,
        borderRadius: 12,
        justifyContent: 'center',
        alignItems: 'center',
        marginTop: 10,
        marginBottom: 15,
        shadowColor: COLORS.primary,
        shadowOffset: { width: 0, height: 4 },
        shadowOpacity: 0.2,
        shadowRadius: 8,
    },
    primaryButtonText: {
        color: COLORS.white,
        fontSize: 16,
        fontWeight: 'bold',
    },
    secondaryButtonText: {
        color: COLORS.primary,
        textAlign: 'center',
        fontSize: 15,
        fontWeight: '600',
    },
    linkText: {
        color: COLORS.subText,
        textAlign: 'center',
        fontSize: 14,
        marginTop: 10,
    }
});

