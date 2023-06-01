import 'flutter_interop_c_platform_interface.dart';

class FlutterInteropC {
  Future<String?> getPlatformVersion() {
    return FlutterInteropCPlatform.instance.getPlatformVersion();
  }

  Future<String> get versionOpenCV =>
      FlutterInteropCPlatform.instance.getOpenCvVersion;

  void imageProcess((String, String) path) =>
      FlutterInteropCPlatform.instance.imageProcess(path);
}
