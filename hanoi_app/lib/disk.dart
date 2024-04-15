import 'package:flutter/material.dart';

class Disk extends StatelessWidget {
  final int size;
  final Color color;

  const Disk({super.key, required this.size, required this.color});

  @override
  Widget build(BuildContext context) {
    return Container(
      width: size.toDouble() * 32, // Variable width based on disk size
      height: 20,
      decoration: BoxDecoration(
        color: color,
        borderRadius: BorderRadius.circular(10),
        border: Border.all(color: Colors.black),
      ),
    );
  }
}
