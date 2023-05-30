import 'dart:ffi';
import 'dart:io';

import 'package:ffi/ffi.dart';
import 'package:flutter/foundation.dart';
import 'package:flutter/services.dart';

import 'flutter_interop_c_platform_interface.dart';
import 'flutter_interop_c_type.dart';

/// An implementation of [FlutterInteropCPlatform] that uses method channels.
class MethodChannelFlutterInteropC extends FlutterInteropCPlatform {
  /// The method channel used to interact with the native platform.
  @visibleForTesting
  final methodChannel = const MethodChannel('flutter_interop_c');

  @override
  Future<String?> getPlatformVersion() async {
    final version =
        await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }

  @override
  Future<String> get versionOpenCV async {
    final nativeLib = Platform.isAndroid
        ? DynamicLibrary.open('libNative.so')
        : DynamicLibrary.process();

    final version = nativeLib.lookupFunction<TypeInC, TypeInDart>('version');
    return version().toDartString();
  }
}
