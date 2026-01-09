import React, { useState } from 'react';
import { View, TextInput, Text, TouchableOpacity, ActivityIndicator } from 'react-native';
import { router } from 'expo-router';
import { GlobalStyles } from './styles';
import { auth } from '../FirebaseConfig';
import { createUserWithEmailAndPassword } from 'firebase/auth';

export default function RegisterScreen() {
    const [email, setEmail] = useState('');
    const [password, setPassword] = useState('');
    // CHANGE 1: Add Confirm Password State
    const [confirmPassword, setConfirmPassword] = useState('');
    const [loading, setLoading] = useState(false);

    const handleSignUp = async () => {
        // CHANGE 2: Validation Logic
        if (password !== confirmPassword) {
            alert("Passwords do not match!");
            return;
        }
        if (password.length < 6) {
            alert("Password should be at least 6 characters.");
            return;
        }

        setLoading(true);
        try {
            await createUserWithEmailAndPassword(auth, email, password);
            router.replace('/travel');
        } catch (error: any) {
            alert(error.message);
        } finally {
            setLoading(false);
        }
    };

    return (
        <View style={GlobalStyles.centerContainer}>
            <View style={GlobalStyles.card}>
                <Text style={GlobalStyles.title}>Create Account</Text>

                <Text style={GlobalStyles.label}>Email</Text>
                <TextInput
                    style={GlobalStyles.input}
                    value={email}
                    onChangeText={setEmail}
                    autoCapitalize="none"
                    placeholder="Enter your email"
                />

                <Text style={GlobalStyles.label}>Password</Text>
                <TextInput
                    style={GlobalStyles.input}
                    value={password}
                    onChangeText={setPassword}
                    secureTextEntry
                    placeholder="Choose a password"
                />

                {/* CHANGE 3: Add Confirm Password Input */}
                <Text style={GlobalStyles.label}>Confirm Password</Text>
                <TextInput
                    style={GlobalStyles.input}
                    value={confirmPassword}
                    onChangeText={setConfirmPassword}
                    secureTextEntry
                    placeholder="Re-enter password"
                />

                <TouchableOpacity
                    style={GlobalStyles.primaryButton}
                    onPress={handleSignUp}
                    disabled={loading}
                >
                    {loading ? <ActivityIndicator color="#FFF" /> : <Text style={GlobalStyles.primaryButtonText}>Sign Up</Text>}
                </TouchableOpacity>

                <TouchableOpacity onPress={() => router.back()} style={{ marginTop: 15 }}>
                    <Text style={GlobalStyles.linkText}>Already have an account? Log In</Text>
                </TouchableOpacity>
            </View>
        </View>
    );
}
