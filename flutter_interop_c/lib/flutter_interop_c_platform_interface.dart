import 'package:plugin_platform_interface/plugin_platform_interface.dart';

import 'flutter_interop_c_method_channel.dart';

abstract class FlutterInteropCPlatform extends PlatformInterface {
  /// Constructs a FlutterInterop_cPlatform.
  FlutterInteropCPlatform() : super(token: _token);

  static final Object _token = Object();

  static FlutterInteropCPlatform _instance = MethodChannelFlutterInteropC();

  /// The default instance of [FlutterInteropCPlatform] to use.
  ///
  /// Defaults to [MethodChannelFlutterInteropC].
  static FlutterInteropCPlatform get instance => _instance;

  /// Platform-specific implementations should set this with their own
  /// platform-specific class that extends [FlutterInteropCPlatform] when
  /// they register themselves.
  static set instance(FlutterInteropCPlatform instance) {
    PlatformInterface.verifyToken(instance, _token);
    _instance = instance;
  }

  Future<String?> getPlatformVersion() {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }

  Future<String> get versionOpenCV {
    throw UnimplementedError('platformVersion() has not been implemented.');
  }
}
