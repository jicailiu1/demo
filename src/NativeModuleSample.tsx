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
 
  key:string = "10159555618571518?s_bl=1&s_l=1&s_psm=1&s_sc=10159555618606518&s_sw=0&s_vt=api_dev&a=AbwE_aW8fbZXXS2F";

  eventHandler(result) {
    console.log("Event was fired with: " + result);
  }

  componentDidMount() {
   }
 
   componentWillUnmount() {
   }
 

   _onPressStart(key) {
     // Calling FancyMath.add method
     NativeModules.MediaStreaming.start(key);
   }

   _onPressCapture() {
    // Calling FancyMath.add method
    NativeModules.MediaStreaming.capture();
  }
 
   render() {
     return (
       <View>
          <Text>key = {this.key}</Text>
          <Button onPress={() => this._onPressStart(this.key)} title="Click me!"/>
          <Button onPress={() => this._onPressCapture()} title="capture"/>
       </View>);
   }
 }
 
 AppRegistry.registerComponent('NativeModuleSample', () => NativeModuleSample);

 export default NativeModuleSample;
