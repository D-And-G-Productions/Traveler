import React, { useState } from 'react';
import { View, TextInput, Button, Text, SafeAreaView } from 'react-native';
import { getAuth } from '@firebase/auth';
import { auth } from '../FirebaseConfig';
import { router } from 'expo-router';
import { GlobalStyles } from './styles';

export default function TravelFormScreen() {
    getAuth().onAuthStateChanged((user) => {
        if (!user) router.replace('/');
    });


    // 1. All states as strings
    const [start, setStart] = useState<string>('');
    const [dest, setDest] = useState<string>('');
    const [time, setTime] = useState<string>('');

    const submitTrip = async () => {
        // 2. Prepare string-based payload
        const payload = {
            start: start,
            destination: dest,
            arrivalTime: time ? new Date(time).toISOString() : '',
        };

        console.log('Sending String Payload:', payload);

        /*
        await fetch('https://api.example.com/trips', {
          method: 'POST',
          headers: { 'Content-Type': 'application/json' },
          body: JSON.stringify(payload),
        });
        */
    };

    return (
        <SafeAreaView style={GlobalStyles.container}>
            <Text style={GlobalStyles.label}>Start Point</Text>
            <TextInput
                style={GlobalStyles.input}
                value={start}
                onChangeText={setStart}
                placeholder="Enter city or address"
            />

            <Text style={GlobalStyles.label}>Destination</Text>
            <TextInput
                style={GlobalStyles.input}
                value={dest}
                onChangeText={setDest}
                placeholder="Enter city or address"
            />

            <Text style={GlobalStyles.label}>Arrival Time</Text>
            <input
                type="datetime-local"
                style={GlobalStyles.webPicker}
                onChange={(e) => setTime(e.target.value)}
            />

            <Button title="Submit Trip" onPress={submitTrip} />
            <Button title="Sign Out" onPress={() => auth.signOut()} />
        </SafeAreaView>
    );
}
