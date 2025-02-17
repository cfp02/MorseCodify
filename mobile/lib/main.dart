import 'package:flutter/material.dart';
import 'screens/device_scan.dart';
import 'screens/morse_screen.dart';

void main() {
  runApp(const MorseCodifyApp());
}

class MorseCodifyApp extends StatelessWidget {
  const MorseCodifyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'MorseCodify',
      theme: ThemeData(
        colorScheme: ColorScheme.fromSeed(
          seedColor: const Color(0xFF2196F3), // Material Blue
          brightness: Brightness.light,
        ),
        useMaterial3: true,
        // Custom theme settings
        appBarTheme: const AppBarTheme(
          centerTitle: true,
          elevation: 0,
        ),
        // Input decoration theme
        inputDecorationTheme: InputDecorationTheme(
          border: OutlineInputBorder(
            borderRadius: BorderRadius.circular(12),
          ),
          focusedBorder: OutlineInputBorder(
            borderRadius: BorderRadius.circular(12),
            borderSide: const BorderSide(
              color: Color(0xFF2196F3),
              width: 2,
            ),
          ),
        ),
        // Elevated button theme
        elevatedButtonTheme: ElevatedButtonThemeData(
          style: ElevatedButton.styleFrom(
            padding: const EdgeInsets.symmetric(
              horizontal: 24,
              vertical: 12,
            ),
            shape: RoundedRectangleBorder(
              borderRadius: BorderRadius.circular(12),
            ),
          ),
        ),
      ),
      home: const DeviceScanScreen(),
      routes: {
        '/morse': (context) => const MorseScreen(),
      },
    );
  }
}
