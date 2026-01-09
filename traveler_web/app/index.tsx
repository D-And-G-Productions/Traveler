import React, { useState } from 'react';
import { View, TextInput, Text, TouchableOpacity, ActivityIndicator } from 'react-native';
import { router } from 'expo-router';
import { GlobalStyles } from './styles';
import { auth } from '../FirebaseConfig';
import { signInWithEmailAndPassword, sendPasswordResetEmail } from 'firebase/auth';

export default function LoginScreen() {
    const [email, setEmail] = useState('');
    const [password, setPassword] = useState('');
    const [loading, setLoading] = useState(false);

    const handleLogin = async () => {
        setLoading(true);
        try {
            await signInWithEmailAndPassword(auth, email, password);
            router.replace('/travel');
        } catch (error: any) {
            alert(error.message);
        } finally {
            setLoading(false);
        }
    };

    const resetPassword = async () => {
        if (!email) return alert("Please enter your email first.");
        try {
            await sendPasswordResetEmail(auth, email);
            alert('Reset email sent.');
        } catch (error: any) {
            alert(error.message);
        }
    };

    return (
        <View style={GlobalStyles.centerContainer}>
            <View style={GlobalStyles.card}>
                <Text style={GlobalStyles.title}>Traveler Login</Text>

                <Text style={GlobalStyles.label}>Email</Text>
                <TextInput
                    style={GlobalStyles.input}
                    value={email}
                    onChangeText={setEmail}
                    autoCapitalize="none"
                />

                <Text style={GlobalStyles.label}>Password</Text>
                <TextInput
                    style={GlobalStyles.input}
                    value={password}
                    onChangeText={setPassword}
                    secureTextEntry
                />

                <TouchableOpacity
                    style={GlobalStyles.primaryButton}
                    onPress={handleLogin}
                    disabled={loading}
                >
                    {loading ? <ActivityIndicator color="#FFF" /> : <Text style={GlobalStyles.primaryButtonText}>Log In</Text>}
                </TouchableOpacity>

                {/* UPDATED BUTTON: Navigates to Register Screen */}
                <TouchableOpacity onPress={() => router.push('/register')} style={{ marginTop: 15 }}>
                    <Text style={GlobalStyles.secondaryButtonText}>Create an Account</Text>
                </TouchableOpacity>

                <TouchableOpacity onPress={resetPassword}>
                    <Text style={GlobalStyles.linkText}>Forgot Password?</Text>
                </TouchableOpacity>
            </View>
        </View>
    );
}
