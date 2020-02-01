/**
 * Sample React Native App
 * https://github.com/facebook/react-native
 *
 * Converted from https://github.com/facebook/react-native/blob/724fe11472cb874ce89657b2c3e7842feff04205/template/App.js
 * With a few tweaks
 */
open ReactNative;
open LocationManager;

type reactNativeNewAppScreenColors = {
  .
  "primary": string,
  "white": string,
  "lighter": string,
  "light": string,
  "black": string,
  "dark": string,
};

[@bs.module "react-native/Libraries/NewAppScreen"]
external colors: reactNativeNewAppScreenColors = "Colors";

[@bs.module "react-native/Libraries/Core/Devtools/openURLInBrowser"]
external openURLInBrowser: string => unit = "default";

module Header = {
  [@react.component] [@bs.module "react-native/Libraries/NewAppScreen"]
  external make: (~txt: string) => React.element = "Header";
};

let rationale =
  PermissionsAndroid.rationale(
    ~title = "Want Loc",
    ~message = "We want to use your fine, fine locations!",
    ~buttonPositive = "OK",
    ~buttonNegative = "No way",
    ~buttonNeutral = "Uh?", ());

/*
 Here is StyleSheet that is using Style module to define styles for your components
 The main different with JavaScript components you may encounter in React Native
 is the fact that they **must** be defined before being referenced
 (so before actual component definitions)
 More at https://reasonml-community.github.io/reason-react-native/en/docs/apis/Style/
 */
let styles =
  Style.(
    StyleSheet.create({
      "scrollView": style(~backgroundColor=colors##lighter, ()),
      "engine": style(~position=`absolute, ~right=0.->dp, ()),
      "body": style(~backgroundColor=colors##white, ()),
      "sectionContainer":
        style(~marginTop=32.->dp, ~paddingHorizontal=24.->dp, ()),
      "sectionTitle":
        style(~fontSize=24., ~fontWeight=`_600, ~color=colors##black, ()),
      "sectionDescription":
        style(
          ~marginTop=8.->dp,
          ~fontSize=18.,
          ~fontWeight=`_400,
          ~color=colors##dark,
          (),
        ),
      "highlight": style(~fontWeight=`_700, ()),
      "footer":
        style(
          ~color=colors##dark,
          ~fontSize=12.,
          ~fontWeight=`_600,
          ~padding=4.->dp,
          ~paddingRight=12.->dp,
          ~textAlign=`right,
          (),
        ),
    })
  );

module BlockStack = {
  [@react.component]
  let make = () => {
    <View style={styles##sectionContainer}>
      <Text style={styles##sectionTitle}>
        "BlockStack"->React.string
      </Text>
      <Text style={styles##sectionDescription}>
        "Log in..."->React.string
      </Text>
    </View>
  }
};

module LocationTracker = {
  [@react.component]
  let make = () => {

    let (trackingLoc, setTrackingLoc) = React.useState(() => false);
    let (watchId, setWatchId) = React.useState(() => -1);

    let setupLocationTracking = () => {

      // Js.log(GeoLocation.getCurrentPosition);
      if (trackingLoc) {
        setTrackingLoc(_ => false);
        Js.log("set to false");
      } else {
        open Js.Result;
        let p1 = PermissionsAndroid.requestWithRationale(PermissionsAndroid.Permission.accessFineLocation, rationale);
        let p2 =
          p1
          ->Promise.Js.fromBsPromise
          ->Promise.Js.toResult;

        p2->Promise.get(fun
          | Ok(v) => {
              Js.log("ok");
              Js.log(v);
              if (v == PermissionsAndroid.Result.granted) {
                setTrackingLoc(_ => true);
                ()
              } else {
                setTrackingLoc(_ => false);
              };
            }
          | Error(e) => {
              Js.log(e);
              setTrackingLoc(_ => false);
            });
      };
    };

    React.useEffect1(
      () => {
        setTrackingLoc(_ => trackingLoc);
        Js.log("change");
        Js.log(trackingLoc);

        if (trackingLoc) {

          let w = makeWatchPosition(
            { pos => {
                Js.log("success");
                Js.log(pos##coords##longitude);
                Js.log(pos##coords##latitude);
                ()
              };
            },
            {_ => Js.log("error")},
            ~timeout=20000.0,
            ~maximumAge=10000.0,
            ~enableHighAccuracy=true,
            ~distanceFilter=0.0,
            ~useSignificantChanges=false
          );
          switch (w) {
          | exception exc => Js.log(exc)
          | n => {
                  Js.log("watchPosition init");
                  Js.log(n);
                  setWatchId(_ => n);
                  }
          };
          ()
        } else {

          clearWatch(watchId);
          Js.log("Watch position cancelled.");

        };
        None
      },
      [|trackingLoc|],
    );

    let stateString =
      if (trackingLoc)
      {
        "Geolocation service IS ACTIVE - touch to deactivate."->React.string;
      } else {
        "Geolocation service is inactive - touch to ACTIVATE."->React.string;
      };

    // ...

      <TouchableOpacity
        onPress={_ => setupLocationTracking()} >
        <Text style={styles##sectionDescription}>
          {stateString}
        </Text>
      </TouchableOpacity>

  };
};

module MainComponent = {
  [@react.component]
  let make = () => {
    <SafeAreaView>
          <ScrollView
            contentInsetAdjustmentBehavior=`automatic style={styles##scrollView}>
            <Header txt="OnMyWay"/>
            <View style={styles##body}>
              <BlockStack />
              <View style={styles##sectionContainer}>
                <Text style={styles##sectionTitle}>
                  "Geolocation"->React.string
                </Text>
                <LocationTracker />
              </View>
            </View>
          </ScrollView>
        </SafeAreaView>
  };
};


[@react.component]
let app = () => {
  <>
    <StatusBar barStyle=`darkContent />
    <MainComponent />
  </>;
};
