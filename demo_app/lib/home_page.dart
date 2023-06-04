import 'dart:async';
import 'dart:developer';
import 'dart:io';
import 'dart:isolate';

import 'package:flutter/material.dart';
import 'package:flutter_interop_c/flutter_interop_c.dart';
import 'package:google_mlkit_text_recognition/google_mlkit_text_recognition.dart';
import 'package:image_picker/image_picker.dart';
import 'package:path_provider/path_provider.dart';

class MyHomePage extends StatefulWidget {
  const MyHomePage({super.key});

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  final nativeOpenCV = FlutterInteropC();

  final _picker = ImagePicker();

  bool _isProcessed = false;
  bool _isWorking = false;

  final textRecognizer = TextRecognizer(script: TextRecognitionScript.latin);

  String? _text;

  Future<String?> pickAnImage() async {
    return _picker
        .pickImage(
          source: ImageSource.gallery,
          imageQuality: 100,
        )
        .then((v) => v?.path);
  }

  Future<String> getTempPath() {
    return getTemporaryDirectory().then((v) => '${v.path}/temp.jpg');
  }

  void takeAndProcessImage() async {
    final imagePath = await pickAnImage();

    if (imagePath == null) return;

    setState(() {
      _isWorking = true;
    });

    final tempPath = await getTempPath();
    final args = (imagePath, tempPath);

    log(tempPath, name: 'Temp Path');
    // Creating a port for communication with isolate and arguments for entry point
    final port = ReceivePort();
    Isolate.spawn<(String, String)>(
      nativeOpenCV.imageProcess,
      args,
      onError: port.sendPort,
      onExit: port.sendPort,
    );

    // Making a variable to store a subscription in
    late StreamSubscription sub;

    // Listening for messages on port
    sub = port.listen((_) async {
      // Cancel a subscription after message received called
      await sub.cancel();

      setState(() {
        _isProcessed = true;
        _isWorking = false;
      });

      final text = await textRecognizer
          .processImage(InputImage.fromFile(File(tempPath)));
      setState(() {
        _text = text.text;
      });
    });

    // final text = await FirebaseMLApi.recogniseText(File(tempPath));
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      appBar: AppBar(
        backgroundColor: Theme.of(context).colorScheme.inversePrimary,
        centerTitle: true,
        title: const Text('Demo OpenCV'),
      ),
      body: Stack(
        children: [
          Center(
            child: ListView(
              shrinkWrap: true,
              children: [
                if (_isProcessed && !_isWorking)
                  ConstrainedBox(
                    constraints:
                        const BoxConstraints(maxWidth: 3000, maxHeight: 300),
                    child: FutureBuilder(
                        future: getTempPath(),
                        builder: (context, snapshot) {
                          if (snapshot.connectionState ==
                              ConnectionState.waiting) {
                            return const CircularProgressIndicator.adaptive();
                          }
                          return Image.file(
                            File(snapshot.data!),
                            alignment: Alignment.center,
                          );
                        }),
                  ),
                Center(
                  child: Column(
                    mainAxisAlignment: MainAxisAlignment.center,
                    children: [
                      Text(_text ?? ''),
                      ElevatedButton(
                        onPressed: takeAndProcessImage,
                        child: const Text('Upload Image'),
                      ),
                      ElevatedButton(
                        onPressed: () {},
                        child: const Text('Get OpenCV Version'),
                      ),
                    ],
                  ),
                ),
              ],
            ),
          ),
          if (_isWorking)
            Positioned.fill(
              child: Container(
                color: Colors.black.withOpacity(.7),
                child: const Center(
                  child: CircularProgressIndicator(),
                ),
              ),
            ),
        ],
      ),
    );
  }
}
