<div align="center">

# 🏛️ Lost Temple

**A modular, high-performance top-down collector game built with Unreal Engine 5, modern C++, and Blueprints.**

[![Unreal Engine](https://img.shields.io/badge/Engine-Unreal_Engine_5.5+-0E1128?logo=unrealengine&logoColor=white)](#prerequisites)
[![Language](https://img.shields.io/badge/Language-C++20%20%7C%20Blueprints-00599C?logo=cplusplus&logoColor=white)](#tech-stack)
[![License: MIT](https://img.shields.io/badge/License-MIT-green.svg)](LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows_x64-blue)](#installation--setup)

<p align="center">
  <a href="#about-the-project">About</a> •
  <a href="#core-features--architecture">Architecture</a> •
  <a href="#prerequisites">Prerequisites</a> •
  <a href="#installation--setup">Setup</a> •
  <a href="#development-roadmap">Roadmap</a> •
  <a href="#contributing">Contributing</a> •
  <a href="#license">License</a>
</p>

</div>

---

## 📖 About the Project

**Lost Temple** is a portfolio-grade game prototype designed to showcase clean software engineering practices within Unreal Engine. The project balances extensible **C++ core systems** (data models, interfaces, gameplay components, event delegates) with designer-friendly **Blueprints** (visual effects, UI bindings, level scripting).

### Key Architectural Highlights
- **Decoupled Architecture:** Event-driven communication through Dynamic Multicast Delegates.
- **Interface-Driven Design:** Flexible interaction pipelines via native C++ interfaces.
- **Data-Driven Workflows:** Gameplay item stats and balancing driven by DataTables.
- **Modern UE5 Memory Management:** Built with `TObjectPtr`, explicit forward declarations, and subobject safety.

---

## 🛠️ Tech Stack & Systems

| Layer | Implementation | Purpose |
| :--- | :--- | :--- |
| **Engine** | Unreal Engine 5.5+ | World composition, lighting, rendering |
| **Core Logic** | Modern C++ (C++20 standard) | High-performance actor classes, data structures |
| **Scripting / VFX** | Blueprints & Niagara | Audio cues, particle spawning, prototyping |
| **Input** | Enhanced Input System | Modular mapping contexts and fluid camera controls |
| **UI** | UMG (Unreal Motion Graphics) | Reactive HUD bound to gameplay delegates |

---

## 📋 Prerequisites

Before setting up the repository, ensure you have the following installed:

1. **Unreal Engine 5.5+** (via Epic Games Launcher or source build).
2. **Visual Studio 2022** (v17.8 or newer) or **JetBrains Rider**.
   - Required VS Workload: *Game development with C++*.
   - Ensure *Unreal Engine Installer* and *MSVC v143 toolsets* are checked.
3. **Git** with **Git LFS (Large File Storage)** enabled.

---

## 🚀 Installation & Setup

Follow these exact steps to clone, generate project files, and compile the workspace:

### 1. Clone the Repository
Make sure Git LFS is initialized before cloning to pull large binary assets (meshes, textures, audio):

```bash
# Initialize Git Large File Storage
git lfs install

# Clone repository
git clone [https://github.com/ibrahim0enes/lost-temple.git](https://github.com/ibrahim0enes/lost-temple.git)
cd lost-temple
