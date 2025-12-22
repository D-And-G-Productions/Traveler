import React from 'react';
import { View, TextInput, Button, Text, StyleSheet } from 'react-native';
import { useRouter } from 'expo-router';
import { useUser } from '../context/UserContext';
import { GlobalStyles } from './styles';

export default function LoginScreen() {
    const router = useRouter();
    const { email, setEmail } = useUser();

    const handleLogin = () => {
        if (email.includes('@')) {
            router.replace('/travel'); // Moves to travel.tsx
        }
    };

    return (
        <View style={GlobalStyles.centerContainer}>
            <View style={GlobalStyles.card}>
                <Text style={GlobalStyles.title}>Welcome</Text>
                <TextInput style={GlobalStyles.input} placeholder="Email" value={email} onChangeText={setEmail} />
                <Button title="Login" onPress={handleLogin} />
            </View>
        </View>
    );
}
