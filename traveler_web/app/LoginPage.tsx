import React, { useState } from 'react';
import { View, TextInput, Button, Text, StyleSheet } from 'react-native';
import { StackNavigationProp } from '@react-navigation/stack';
import { RootStackParamList } from './MainPage';
import styles from './styles';

// --- Login Page ---

type LoginScreenProps = {
    navigation: StackNavigationProp<RootStackParamList, 'Login'>;
};

const LoginScreen = ({ navigation }: LoginScreenProps) => {
    const [email, setEmail] = useState('');

    const handleLogin = () => {
        if (email.includes('@')) {
            navigation.navigate('TravelForm', { userEmail: email });
        }
    };

    return (
        <View style={styles.centerContainer}>
            <View style={styles.card}>
                <Text style={styles.title}>Welcome</Text>
                <TextInput
                    style={styles.input}
                    placeholder="Email"
                    value={email}
                    onChangeText={setEmail}
                />
                <Button title="Login" onPress={handleLogin} />
            </View>
        </View>
    );
};

export default LoginScreen;

