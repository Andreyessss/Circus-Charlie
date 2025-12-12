# 🎪 Circus Charlie

Implementación del clásico juego Circus Charlie usando C++, SFML y Box2D.

## 📋 Descripción del Proyecto

Circus Charlie es un juego arcade donde controlas a un león que debe saltar sobre aros de fuego mientras avanza por el escenario del circo. Este proyecto está desarrollado como parte de la materia Proyecto 252.

## 🎯 Objetivo del Juego

Controla al león Charlie y evita los obstáculos (aros de fuego) que vienen hacia ti. Sobrevive el mayor tiempo posible y obtén la puntuación más alta.

## 🎮 Controles

### En el Menú
- **Flechas Arriba/Abajo**: Navegar opciones
- **Enter**: Seleccionar opción

### En el Juego
- **A / Flecha Izquierda**: Mover a la izquierda
- **D / Flecha Derecha**: Mover a la derecha
- **Espacio / W / Flecha Arriba**: Saltar
- **ESC**: Volver al menú

### En Game Over
- **R**: Reiniciar juego
- **M / ESC**: Volver al menú

## ⚙️ Mecánicas

- **Sistema de Vidas**: Comienzas con 3 vidas
- **Puntuación**: Ganas 10 puntos por cada obstáculo evitado
- **Dificultad Progresiva**: Los obstáculos aparecen cada vez más rápido
- **Invulnerabilidad**: Después de recibir daño, eres invulnerable por 2 segundos

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

### Texturas (opcional, el juego usa figuras geométricas por defecto)
- `assets/textures/background.png` - Fondo del juego
- `assets/textures/player.png` - Sprite del jugador
- `assets/textures/obstacle.png` - Sprite del obstáculo

### Sonidos (opcional)
- `assets/sounds/music.ogg` - Música de fondo
- `assets/sounds/jump.wav` - Sonido de salto
- `assets/sounds/hit.wav` - Sonido de colisión

**Nota**: El juego funcionará sin estos assets usando formas geométricas y sin sonido.

## 🏆 Características

- ✅ Menú principal funcional
- ✅ Nivel 1 completo
- ✅ Sistema de física con Box2D
- ✅ Detección de colisiones
- ✅ Sistema de vidas
- ✅ Sistema de puntuación
- ✅ Dificultad progresiva
- ✅ Invulnerabilidad temporal
- ✅ Game Over screen
- ✅ Controles intuitivos

## 👥 Equipo

- **Tu Nombre**: [@tu-usuario-github](https://github.com/tu-usuario)

## 📜 Créditos

Inspirado en el clásico arcade Circus Charlie de Konami (1984).

## 📝 Licencia

Este proyecto es de código abierto y está disponible para fines educativos.

---

**Proyecto 252** - Universidad de Guadalajara