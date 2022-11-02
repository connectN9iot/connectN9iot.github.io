// Import the functions you need from the SDKs you need
//import { initializeApp } from "https://www.gstatic.com/firebasejs/9.5.0/firebase-app.js";
//import { initializeApp } from "firebase/app";
import { getDatabase, ref, set, child, get } from "https://www.gstatic.com/firebasejs/9.5.0/firebase-database.js";
//import { getAnalytics } from "firebase/analytics";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries
import { initializeApp } from "https://bysoctrang-default-rtdb.asia-southeast1.firebasedatabase.app/";
// Your web app's Firebase configuration
//const firebaseConfig = {
  // Import the functions you need from the SDKs you need
//import { initializeApp } from "firebase/app";
//import { getAnalytics } from "firebase/analytics";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
// For Firebase JS SDK v7.20.0 and later, measurementId is optional

// Import the functions you need from the SDKs you need
//import { initializeApp } from "firebase/app";
// TODO: Add SDKs for Firebase products that you want to use
// https://firebase.google.com/docs/web/setup#available-libraries

// Your web app's Firebase configuration
const firebaseConfig = {
  apiKey: "AIzaSyD4pt30fnjFCwN_UHLLme-CJCn8qwPjChA",
  authDomain: "bysoctrang.firebaseapp.com",
  databaseURL: "https://bysoctrang-default-rtdb.asia-southeast1.firebasedatabase.app",
  projectId: "bysoctrang",
  storageBucket: "bysoctrang.appspot.com",
  messagingSenderId: "452399304042",
  appId: "1:452399304042:web:80bc78d8deecc279ad6913"
};

// Initialize Firebase
const app = initializeApp(firebaseConfig);

  //Điền thông tin Firebase config của cá nhân vào đây
  
//};

// Initialize Firebase


send.addEventListener('click',(e) =>{ 
Sends();
});
read.addEventListener('click',(e) =>{ 
Getval();
});

function Sends(){
    Number = parseFloat(document.getElementById('Number').value);
const db = getDatabase();
  set(ref(db, 'Number/'), {
    Value : Number
  });
}

function Getval(){
    const dbRef = ref(getDatabase());
    get(child(dbRef, `Number` + '/Value')).then((snapshot) => {
      if (snapshot.exists()) {
        console.log(snapshot.val());
        document.getElementById('val').innerHTML = "Number/Value="+snapshot.val();
      } else {
        console.log("No data available");
      }
    }).catch((error) => {
      console.error(error);
    });
}
