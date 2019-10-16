type coords = {
  .
  "speed": int,
  "longitude": float,
  "latitude": float,
  "heading": int,
  "altitude": float,
  "accuracy": float,
};

type position = {
  .
  "coords": coords,
  "mocked": bool,
  "timestamp": float,
};

type error = {
  .
  "code": int,
  "message": string,
};

[@bs.deriving abstract]
type successCb = position => unit;

[@bs.deriving abstract]
type errorCb = error => unit;

type currentPositionOptions;
[@bs.obj]
external currentPositionOptions:
  (
    ~timeout: float=?, 
    ~maximumAge: float=?, 
    ~enableHighAccuracy: bool=?) =>
  currentPositionOptions =
  "";

[@bs.val] [@bs.module "react-native-geolocation-service"] [@bs.scope "default"]
external getCurrentPosition:
  (successCb, errorCb, currentPositionOptions) => unit =
  "getCurrentPosition";

let makeCurrentPosition =
    (~timeout=?, ~maximumAge=?, ~enableHighAccuracy=?, success, error) =>
  getCurrentPosition(
    success,
    error,
    currentPositionOptions(~timeout?, ~maximumAge?, ~enableHighAccuracy?),
  );

type watchId = int;

type watchPositionOptions;
[@bs.obj]
external watchPositionOptions:
  (
    ~timeout: float=?,
    ~maximumAge: float=?,
    ~enableHighAccuracy: bool=?,
    ~distanceFilter: float=?,
    ~useSignificantChanges: bool=?) =>
  watchPositionOptions =
  "";

[@bs.val] [@bs.module "react-native-geolocation-service"] [@bs.scope "default"]
external watchPosition:
  (successCb, errorCb, watchPositionOptions) => watchId =
  "";

let makeWatchPosition =
    (~timeout=?, ~maximumAge=?, ~enableHighAccuracy=?, ~distanceFilter=?, ~useSignificantChanges=?, success, error) =>
  watchPosition(
    success,
    error,
    watchPositionOptions(
      ~timeout?, 
      ~maximumAge?, 
      ~enableHighAccuracy?,
      ~distanceFilter?, 
      ~useSignificantChanges?)
  );

[@bs.val] [@bs.module "react-native-geolocation-service"] [@bs.scope "default"]
external clearWatch: watchId => unit = "";
