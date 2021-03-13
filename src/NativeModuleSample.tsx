/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * @format
 * @flow strict-local
 */


 import React, { Component } from 'react';
 import {
   AppRegistry,
   Alert,
   Text,
   View,
   Button,
 } from 'react-native';
 
 import { NativeModules, NativeEventEmitter } from 'react-native';
 
 const EventEmitter = new NativeEventEmitter(NativeModules.FancyMath);
 
 class NativeModuleSample extends Component {
 
  key:string = "10159539268166518?s_bl=1&s_l=1&s_ps=1&s_psm=1&s_sw=0&s_vt=api_dev&a=AbxjrrkJKTTNAHnX";

  eventHandler(result) {
    console.log("Event was fired with: " + result);
  }

  componentDidMount() {
   }
 
   componentWillUnmount() {
   }
 

   _onPressHandler(key) {
     // Calling FancyMath.add method
     console.log("_onPressHandler " + key);
     NativeModules.MediaStreaming.start(key,
       /* callback */ function (result) {
         Alert.alert("result", `${result}`);
       });
   }
 
   render() {
     return (
       <View>
          <Text>key = {this.key}</Text>
          <Button onPress={() => this._onPressHandler(this.key)} title="Click me!"/>
       </View>);
   }
 }
 
 AppRegistry.registerComponent('NativeModuleSample', () => NativeModuleSample);

 export default NativeModuleSample;
