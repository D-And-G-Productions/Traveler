import React, { useState } from 'react';
import { View, TextInput, Button, Text, StyleSheet, SafeAreaView } from 'react-native';
import { NavigationContainer } from '@react-navigation/native';
import { createStackNavigator, StackNavigationProp } from '@react-navigation/stack';

// --- Interfaces ---

interface User {
    email: string;
}

interface Location {
    address: string;
    placeId: string;
    coords: { lat: number; lng: number };
}

// Define the "routes" and what data they expect
type RootStackParamList = {
    Login: undefined;
    TravelForm: { userEmail: string };
};

const Stack = createStackNavigator<RootStackParamList>();

// --- Screen 1: Login ---

type LoginScreenProps = {
    navigation: StackNavigationProp<RootStackParamList, 'Login'>;
};

const LoginScreen = ({ navigation }: LoginScreenProps) => {
    const [email, setEmail] = useState('');

    const handleLogin = () => {
        if (email.includes('@')) {
            // Navigate to the next page and pass the email
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

// --- Screen 2: Travel Form ---

type FormScreenProps = {
    navigation: StackNavigationProp<RootStackParamList, 'TravelForm'>;
    route: any;
};

const TravelFormScreen = ({ navigation, route }: FormScreenProps) => {
    const { userEmail } = route.params;
    const [start, setStart] = useState<Location>({ address: '', placeId: '', coords: { lat: 0, lng: 0 } });
    const [dest, setDest] = useState<Location>({ address: '', placeId: '', coords: { lat: 0, lng: 0 } });
    const [time, setTime] = useState('');

    return (
        <SafeAreaView style={styles.container}>
            <View style={styles.header}>
                <Text>Logged in as: {userEmail}</Text>
                <Button title="Logout" onPress={() => navigation.goBack()} color="red" />
            </View>

            <TextInput
                style={styles.input}
                placeholder="Start Point"
                onChangeText={(t) => setStart({ ...start, address: t })}
            />
            <TextInput
                style={styles.input}
                placeholder="Destination"
                onChangeText={(t) => setDest({ ...dest, address: t })}
            />

            <input
                type="datetime-local"
                style={styles.webPicker}
                onChange={(e) => setTime(e.target.value)}
            />

            <Button title="Submit Trip" onPress={() => console.log({ userEmail, start, dest, time })} />
        </SafeAreaView>
    );
};

// --- Main App Component ---

export default function App() {
    return (
        <Stack.Navigator initialRouteName="Login">
            <Stack.Screen name="Login" component={LoginScreen} options={{ headerShown: false }} />
            <Stack.Screen name="TravelForm" component={TravelFormScreen} options={{ title: 'Plan Your Trip' }} />
        </Stack.Navigator>
    );
}

const styles = StyleSheet.create({
    centerContainer: { flex: 1, justifyContent: 'center', alignItems: 'center' },
    container: { flex: 1, padding: 20, maxWidth: 500, alignSelf: 'center', width: '100%' },
    card: { width: 300, padding: 20, backgroundColor: 'white', borderRadius: 8, elevation: 3 },
    title: { fontSize: 20, marginBottom: 20, textAlign: 'center' },
    input: { height: 40, borderBottomWidth: 1, marginBottom: 20 },
    header: { flexDirection: 'row', justifyContent: 'space-between', marginBottom: 20 },
    webPicker: { padding: 10, width: '100%', marginBottom: 20 }
});
