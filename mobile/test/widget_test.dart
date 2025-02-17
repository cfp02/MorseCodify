// This is a basic Flutter widget test.
//
// To perform an interaction with a widget in your test, use the WidgetTester
// utility in the flutter_test package. For example, you can send tap and scroll
// gestures. You can also use WidgetTester to find child widgets in the widget
// tree, read text, and verify that the values of widget properties are correct.

import 'package:flutter/material.dart';
import 'package:flutter_test/flutter_test.dart';

import 'package:mobile/main.dart';
import 'package:mobile/screens/device_scan.dart';

void main() {
  group('MorseCodify App Tests', () {
    testWidgets('App starts with device scan screen',
        (WidgetTester tester) async {
      // Build our app and trigger a frame
      await tester.pumpWidget(const MorseCodifyApp());

      // Verify that we start with the device scan screen
      expect(find.byType(DeviceScanScreen), findsOneWidget);
      expect(find.text('MorseCodify'), findsOneWidget);
      expect(find.text('Available Devices'), findsOneWidget);
    });

    testWidgets('Device scan screen shows empty state correctly',
        (WidgetTester tester) async {
      await tester.pumpWidget(const MorseCodifyApp());

      // Initially should show "No MorseCodify devices found"
      expect(find.text('No MorseCodify devices found'), findsOneWidget);
      expect(find.byIcon(Icons.refresh), findsOneWidget);
    });

    testWidgets('App theme is configured correctly',
        (WidgetTester tester) async {
      await tester.pumpWidget(const MorseCodifyApp());

      // Get the MaterialApp widget
      final MaterialApp app = tester.widget(find.byType(MaterialApp));

      // Verify theme settings
      expect(app.theme?.colorScheme.primary, const Color(0xFF2196F3));
      expect(app.theme?.useMaterial3, true);
    });
  });
}
