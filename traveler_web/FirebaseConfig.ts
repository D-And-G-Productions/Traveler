// Import the functions you need from the SDKs you need
import { initializeApp } from "firebase/app";
import { getAuth } from "firebase/auth";
// const firebaseConfig = {
//     apiKey: "AIzaSyDHK29uOINgapSmoHsg0b_6DxwQX6VyvMA",
//     authDomain: "traveler-a3efb.firebaseapp.com",
//     projectId: "traveler-a3efb",
//     storageBucket: "traveler-a3efb.firebasestorage.app",
//     messagingSenderId: "608904282716",
//     appId: "1:608904282716:web:09203ef65f1a1d8adf9c99"
// };
const firebaseConfig = {
    apiKey: process.env.EXPO_PUBLIC_FIREBASE_API_KEY,
    authDomain: process.env.EXPO_PUBLIC_FIREBASE_AUTH_DOMAIN,
    projectId: process.env.EXPO_PUBLIC_FIREBASE_PROJECT_ID,
    storageBucket: process.env.EXPO_PUBLIC_FIREBASE_STORAGE_BUCKET,
    messagingSenderId: process.env.EXPO_PUBLIC_FIREBASE_MESSAGING_SENDER_ID,
    appId: process.env.EXPO_PUBLIC_FIREBASE_APP_ID
};

// Initialize Firebase
export const app = initializeApp(firebaseConfig);
export const auth = getAuth(app);
