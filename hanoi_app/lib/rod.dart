import 'package:flutter/material.dart';

class Rod extends StatelessWidget {
  final List<Widget> children;

  const Rod({super.key, required this.children});

  @override
  Widget build(BuildContext context) {
    return Container(
      width: 100,
      height: 300,
      padding: const EdgeInsets.all(8),
      decoration: BoxDecoration(
        border: Border.all(color: Colors.blueAccent),
        borderRadius: BorderRadius.circular(12),
      ),
      child: Column(
        mainAxisAlignment: MainAxisAlignment.end,
        children: children,
      ),
    );
  }
}

