import 'dart:ffi';
import 'package:ffi/ffi.dart';

// C function signatures
typedef VersionFuncInC = Pointer<Utf8> Function();
typedef ImageProcessInC = Void Function(Pointer<Utf8>, Pointer<Utf8>);

// Dart function signatures
typedef VersionFuncInDart = Pointer<Utf8> Function();
typedef ImageProcessInDart = void Function(Pointer<Utf8>, Pointer<Utf8>);
