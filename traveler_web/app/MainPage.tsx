import React, { useState } from 'react';
import { View, TextInput, Button, Text, SafeAreaView } from 'react-native';
import { createStackNavigator, StackNavigationProp } from '@react-navigation/stack';
import LoginScreen from './LoginPage';
import styles from './styles';

// --- Interfaces ---

export interface Location {
    address: string;
    placeId: string;
    coords: { lat: number; lng: number };
}

// Define the "routes" and what data they expect
export type RootStackParamList = {
    Login: undefined;
    TravelForm: { userEmail: string };
};

const Stack = createStackNavigator<RootStackParamList>();

// --- Travel Form Screen ---

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

