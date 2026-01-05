import React, { useState } from 'react';
import { View, TextInput, Button, Text } from 'react-native';
import { router } from 'expo-router';
import { GlobalStyles } from './styles';
import { auth } from '../FirebaseConfig';
import { createUserWithEmailAndPassword, signInWithEmailAndPassword } from 'firebase/auth';

export default function LoginScreen() {
    const [email, setEmail] = useState('');
    const [password, setPassword] = useState('');


    const signIn = async () => {
        try {
            const user = await signInWithEmailAndPassword(auth, email, password);
            if (user) router.replace('/travel');
        } catch (error: any) {
            console.log(error);
            alert('Sign in failed: ' + error.message);
        }
    }
    const signUp = async () => {
        try {
            const user = await createUserWithEmailAndPassword(auth, email, password);
            if (user) router.replace('/travel');
        } catch (error: any) {
            console.log(error);
            alert('Sign in failed: ' + error.message);
        }
    }

    return (
        <View style={GlobalStyles.centerContainer}>
            <View style={GlobalStyles.card}>
                <Text style={GlobalStyles.title}>Welcome</Text>
                <TextInput style={GlobalStyles.input} placeholder="Email" value={email} onChangeText={setEmail} />
                <TextInput style={GlobalStyles.input} placeholder="Password" value={password} onChangeText={setPassword} />
                <Button title="Login" onPress={signIn} />
                <Button title="Make an Account" onPress={signUp} />
            </View>
        </View>
    );
}
