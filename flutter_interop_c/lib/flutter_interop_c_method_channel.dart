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

  final nativeLib = Platform.isAndroid
      ? DynamicLibrary.open('libNative.so')
      : DynamicLibrary.process();

  @override
  Future<String?> getPlatformVersion() async {
    final version =
        await methodChannel.invokeMethod<String>('getPlatformVersion');
    return version;
  }

  @override
  Future<String> get getOpenCvVersion async {
    final version =
        nativeLib.lookupFunction<VersionFuncInC, VersionFuncInDart>('version');
    return version().toDartString();
  }

  @override
  void imageProcess((String, String) path) {
    final process = nativeLib
        .lookupFunction<ImageProcessInC, ImageProcessInDart>('process_image');
    process(path.$1.toNativeUtf8(), path.$2.toNativeUtf8());
  }
}
