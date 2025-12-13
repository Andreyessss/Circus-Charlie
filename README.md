# Circus Charlie — Proyecto 252

Este repositorio contiene la versión del proyecto "Circus Charlie" preparada para entregar en la materia Proyecto 252. Sigue la estructura requerida por la rúbrica y contiene el ejecutable, recursos y capturas necesarias.

## Estructura del repositorio

proyecto-252/
- .github/workflows/       ← GitHub Action para publicar (si aplica)
- bin/                    ← Ejecutable del juego (Windows .exe)
- video/                  ← `demo.mp4` (video de gameplay, <= 2 min)
- gallery/                ← `cover.png` (imagen de portada 720x1080)
- screenshots/            ← `screenshot1.png`, `screenshot2.png`, `screenshot3.png`
- assets/
  - textures/             ← imágenes (png, jpg)
  - sounds/               ← efectos y música (ogg, wav)
  - fonts/                ← fuentes (ttf)
  - unused/               ← archivos no referenciados (backup)
- include/                ← headers C++
- src/                    ← código fuente C++ (SFML)
- README.md               ← este archivo
- .gitignore

## Cómo compilar (Windows / MSYS2 - MinGW64)

1. Abrir MSYS2 MinGW64 shell.
2. Ejecutar el comando de compilación:

```bash
g++ -Iinclude -Isrc -g src/*.cpp -o bin/CircusCharlie.exe -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio
```

3. Ejecutar el juego:

```powershell
.\bin\CircusCharlie.exe
```

Nota: ya incluimos el binario de demostración en `bin/`.

## Controles

- Izquierda/Derecha: mover
- Space: saltar
- Up: trepar (cuando estés junto a la torre)

## Qué revisar antes de subir

- `video/demo.mp4` con gameplay (<= 2 min)
- `gallery/cover.png` (720x1080)
- `screenshots/` con al menos 3 capturas (1080x720 cada una)
- `bin/CircusCharlie.exe` incluido
- `assets/` organizado en `textures`,`sounds`,`fonts`

## Créditos

Autores: Josue Emir Gonzalez Plascencia, Andre Reyes Perez

Assets usados: listar en `assets/` (respetar licencias)

## Notas del desarrollador

- Hice una copia de seguridad de las fuentes originales con comentarios en `.backup_sources/`.
- Moví archivos no referenciados a `assets/unused/` para seguridad antes de limpiar.

Si quieres que haga el push al repositorio remoto o que genere el Release ZIP para CETUS, dime y lo preparo (necesitarás ejecutar el commit y push localmente o autorizar al CI).

---
Versión: entrega Proyecto 252 — estructura y README actualizados.
# 🎪 Circus Charlie

Implementación del clásico juego Circus Charlie usando C++, SFML y Box2D.

## 📋 Descripción del Proyecto

Circus Charlie es un juego arcade donde controlas a un león que debe saltar sobre aros de fuego mientras avanza por el escenario del circo. Este proyecto está desarrollado como parte de la materia Proyecto 252.

## 🎯 Objetivo del Juego

Controla al león Charlie (en este caso es el mota puercos) y evita los obstáculos (aros de fuego) que vienen hacia ti. Sobrevive el mayor tiempo posible y llega a la meta.

## 🎮 Controles

### En el Menú
- **Enter**: Seleccionar jugador

### En el Juego
- **Flecha Izquierda**: Mover a la izquierda
- **Flecha Derecha**: Mover a la derecha
- **Espacio /**: Saltar
- **ESC**: Salir

### En Game Over
- **Espacio**: Reiniciar juego
- **ESC**: Salir

## ⚙️ Mecánicas

- **Sistema de Vidas**: Comienzas con 3 vidas
- **Puntuación**: Ganas 100 puntos por cada obstáculo saltado

## 🛠️ Tecnologías

- **Lenguaje**: C++17
- **Gráficos**: SFML 2.5+
- **Física**: Box2D 3.x
- **Build System**: Make

## 📦 Requisitos

### Windows (MSYS2 MINGW64)

```bash
# Instalar herramientas de desarrollo
pacman -S --needed base-devel mingw-w64-x86_64-toolchain

# Instalar SFML
pacman -S mingw-w64-x86_64-sfml

# Instalar Box2D
pacman -S mingw-w64-x86_64-box2d

# Instalar Make
pacman -S make
```

### Linux (Ubuntu/Debian)

```bash
sudo apt-get install libsfml-dev
sudo apt-get install libbox2d-dev
sudo apt-get install build-essential
```

### macOS (Homebrew)

```bash
brew install sfml
brew install box2d
```

## 🔨 Compilación

### Compilar el proyecto

```bash
make
```

### Ejecutar el juego

```bash
make run
```

### Limpiar archivos compilados

```bash
make clean
```

### Recompilar desde cero

```bash
make rebuild
```

## 📁 Estructura del Proyecto

```
CircusCharlie/
├── include/          # Headers (.h)
│   ├── Game.h
│   ├── Player.h
│   ├── Platform.h
│   └── Obstacle.h
├── src/              # Implementación (.cpp)
│   ├── main.cpp
│   ├── Game.cpp
│   ├── Player.cpp
│   ├── Platform.cpp
│   └── Obstacle.cpp
├── assets/           # Recursos del juego
│   ├── textures/
│   ├── sounds/
│   └── fonts/
├── bin/              # Ejecutable
├── obj/              # Archivos objeto
├── gallery/          # Portada del proyecto
│   └── cover.png
├── screenshots/      # Capturas del juego
├── video/            # Video de gameplay
│   └── demo.mp4
├── .github/
│   └── workflows/
│       └── publish.yml
├── makefile
├── README.md
└── .gitignore
```

## 🎨 Assets Necesarios

Para que el juego funcione completamente con gráficos y sonidos, necesitas agregar los siguientes archivos:

### Fuentes
- `assets/fonts/arial.ttf` - Fuente para el texto
  `assets/PRESSstart2P.ttf` - Fuente para el texto del juego

### Texturas (opcional, el juego usa figuras geométricas por defecto)
- `assets/textures/background.png` - Fondo del juego
- `assets/textures/player.png` - Sprite del jugador
- `assets/textures/obstacle.png` - Sprite del obstáculo

### Sonidos (opcional)
- `assets/menu.ogg` - Música del menu
- `assets/level.ogg` - Música de nivel
- `assets/jump.wav` - Sonido de salto


**Nota**: El juego funcionará sin estos assets usando formas geométricas y sin sonido.

## 🏆 Características

- ✅ Menú principal funcional
- ✅ Nivel 1 completo
- ✅ Sistema de física con Box2D
- ✅ Detección de colisiones
- ✅ Sistema de vidas
- ✅ Sistema de puntuación
- ✅ Game Over screen
- ✅ Controles intuitivos

## 👥 Equipo

- **Josue Emir Gonzalez Plascencia**: [a24310306@live.ceti.mx](https://github.com/JosueEmirGonPlasc)
- **Andre Reyes Perez**: [a24310407@live.ceti.mx](https://github.com/Andreyessss)

## 📜 Créditos

Inspirado en el clásico arcade Circus Charlie de Konami (1984).

## 📝 Licencia

Este proyecto es de código abierto y está disponible para fines educativos.

---

**Proyecto 252** - Ceti Colomos