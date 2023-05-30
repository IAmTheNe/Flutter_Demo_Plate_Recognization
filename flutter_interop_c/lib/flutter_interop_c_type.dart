import 'dart:ffi';
import 'package:ffi/ffi.dart';

typedef TypeInC = Pointer<Utf8> Function();
typedef TypeInDart = Pointer<Utf8> Function();