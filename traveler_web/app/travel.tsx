import React, { useState, useEffect } from 'react';
import { View, TextInput, Text, TouchableOpacity, Platform, SafeAreaView, FlatList } from 'react-native';
import { getAuth, onAuthStateChanged } from '@firebase/auth';
import { auth } from '../FirebaseConfig';
import { router } from 'expo-router';
import { GlobalStyles } from './styles';

// 1. Mock Data
const MOCK_TRIPS = [
    { id: '1', start: 'London', dest: 'Paris', date: '2025-08-15' },
    { id: '2', start: 'New York', dest: 'Tokyo', date: '2025-09-10' },
    { id: '3', start: 'Berlin', dest: 'Amsterdam', date: '2025-10-05' },
];

export default function TravelFormScreen() {
    useEffect(() => {
        const unsubscribe = onAuthStateChanged(auth, (user) => {
            if (!user) router.replace('/');
        });
        return unsubscribe;
    }, []);

    const [start, setStart] = useState('');
    const [dest, setDest] = useState('');
    const [time, setTime] = useState('');

    const submitTrip = async () => {
        // ... submission logic ...
        alert(`Trip to ${dest} submitted!`);
    };

    // 2. The Form Component (Rendered as the Header)
    const renderHeader = () => (
        <View style={{ marginBottom: 25 }}>
            <View style={GlobalStyles.card}>
                <Text style={GlobalStyles.title}>Plan New Trip</Text>

                <Text style={GlobalStyles.label}>Start Point</Text>
                <TextInput style={GlobalStyles.input} value={start} onChangeText={setStart} placeholder="e.g. London" />

                <Text style={GlobalStyles.label}>Destination</Text>
                <TextInput style={GlobalStyles.input} value={dest} onChangeText={setDest} placeholder="e.g. Paris" />

                <Text style={GlobalStyles.label}>Arrival Time</Text>
                {Platform.OS === 'web' ? (
                    <input
                        type="datetime-local"
                        style={{ ...GlobalStyles.input, width: '100%', border: 'none', fontFamily: 'system-ui' }}
                        onChange={(e) => setTime(e.target.value)}
                    />
                ) : (
                    <Text style={{ marginBottom: 15, color: '#888' }}>Date selection requires native picker</Text>
                )}

                <TouchableOpacity style={GlobalStyles.primaryButton} onPress={submitTrip}>
                    <Text style={GlobalStyles.primaryButtonText}>Add Trip</Text>
                </TouchableOpacity>

                <TouchableOpacity onPress={() => auth.signOut()} style={{ marginTop: 15 }}>
                    <Text style={{ color: '#FF3B30', textAlign: 'center', fontWeight: '600' }}>Sign Out</Text>
                </TouchableOpacity>
            </View>

            <Text style={[GlobalStyles.title, { marginTop: 30, marginBottom: 10, fontSize: 20 }]}>
                Upcoming Trips
            </Text>
        </View>
    );

    // 1. Action Handlers
    const handleEdit = (item: typeof MOCK_TRIPS[0]) => {
        // Example: Populate form with this trip's data
        setStart(item.start);
        setDest(item.dest);
        alert(`Editing trip to ${item.dest} - details loaded above.`);
    };

    const handleDelete = (id: string) => {
        // Example: logic to remove from state/database
        alert(`Trip ${id} deleted.`);
    };

    // 2. Updated Trip Card
    const renderTrip = ({ item }: { item: typeof MOCK_TRIPS[0] }) => (
        <View style={[GlobalStyles.card, { padding: 20, marginBottom: 15 }]}>

            {/* Trip Info Section */}
            <View style={{ flexDirection: 'row', justifyContent: 'space-between', alignItems: 'center' }}>
                <View>
                    <Text style={{ fontSize: 18, fontWeight: 'bold', color: '#2A2A2A' }}>{item.dest}</Text>
                    <Text style={{ color: '#666', marginTop: 4 }}>From: {item.start}</Text>
                </View>
                <View style={{ backgroundColor: '#E8F0FE', paddingHorizontal: 10, paddingVertical: 6, borderRadius: 8 }}>
                    <Text style={{ color: '#2A2A2A', fontWeight: '600', fontSize: 12 }}>{item.date}</Text>
                </View>
            </View>

            {/* Visual Divider */}
            <View style={{ height: 1, backgroundColor: '#F0F0F0', marginVertical: 15 }} />

            {/* Options "Tab" (Action Row) */}
            <View style={{ flexDirection: 'row', justifyContent: 'flex-end' }}>
                <TouchableOpacity onPress={() => handleEdit(item)} style={{ marginRight: 20 }}>
                    <Text style={{ color: '#007AFF', fontWeight: '600', fontSize: 14 }}>Edit</Text>
                </TouchableOpacity>

                <TouchableOpacity onPress={() => handleDelete(item.id)}>
                    <Text style={{ color: '#FF3B30', fontWeight: '600', fontSize: 14 }}>Delete</Text>
                </TouchableOpacity>
            </View>
        </View>
    );

    return (
        <SafeAreaView style={[GlobalStyles.centerContainer, { justifyContent: 'flex-start' }]}>
            <FlatList
                data={MOCK_TRIPS}
                renderItem={renderTrip}
                keyExtractor={(item) => item.id}
                ListHeaderComponent={renderHeader}
                contentContainerStyle={{ paddingBottom: 40, width: '100%', maxWidth: 1000, alignSelf: 'center' }}
                showsVerticalScrollIndicator={false}
            />
        </SafeAreaView>
    );
}
