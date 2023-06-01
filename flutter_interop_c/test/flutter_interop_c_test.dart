import 'package:flutter_test/flutter_test.dart';
import 'package:flutter_interop_c/flutter_interop_c.dart';
import 'package:flutter_interop_c/flutter_interop_c_platform_interface.dart';
import 'package:flutter_interop_c/flutter_interop_c_method_channel.dart';
import 'package:plugin_platform_interface/plugin_platform_interface.dart';

class MockFlutterInteropCPlatform
    with MockPlatformInterfaceMixin
    implements FlutterInteropCPlatform {
  @override
  Future<String?> getPlatformVersion() => Future.value('42');

  @override
  Future<String> get getOpenCvVersion => throw UnimplementedError();
}

void main() {
  final FlutterInteropCPlatform initialPlatform =
      FlutterInteropCPlatform.instance;

  test('$MethodChannelFlutterInteropC is the default instance', () {
    expect(initialPlatform, isInstanceOf<MethodChannelFlutterInteropC>());
  });

  test('getPlatformVersion', () async {
    FlutterInteropC flutterInterop_cPlugin = FlutterInteropC();
    MockFlutterInteropCPlatform fakePlatform = MockFlutterInteropCPlatform();
    FlutterInteropCPlatform.instance = fakePlatform;

    expect(await flutterInterop_cPlugin.getPlatformVersion(), '42');
  });
}
