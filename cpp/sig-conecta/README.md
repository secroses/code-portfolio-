# SIG-Conecta — Sistema de Gestión de Equipos

**Materia:** Programación Estructurada · UNRC · Semestre 2  
**Actividad:** Integradora Final (Módulos M2–M4)  
**Lenguaje:** C++ estándar (sin librerías externas)

---

## 📋 Descripción

SIG-Conecta es un sistema de consola que permite administrar un laboratorio comunitario de 20 equipos de cómputo en entornos **sin conexión a internet ni acceso a servicios en la nube**.

El problema central: en zonas con infraestructura eléctrica inestable, cualquier dato guardado solo en RAM se pierde ante un corte de energía. Este sistema resuelve eso mediante **persistencia en archivos de texto plano**, recuperando automáticamente el estado completo del laboratorio al reiniciar.

---

## ⚙️ Funcionalidades

- Consultar el estado de los 20 equipos (Libre / Ocupada / Mantenimiento)
- Modificar el estado de cualquier equipo con validación de entrada
- Registrar incidentes con hora, tipo y descripción
- Consultar el historial completo de incidentes en pantalla
- Guardar todo en `bitacora.txt` y recuperarlo automáticamente al iniciar

---

## 🧱 Decisiones técnicas destacadas

**`struct RegistroBitacora`** en lugar de una matriz `string[100][4]`  
→ Seguridad de tipos: el campo `idEquipo` es `int`, no texto. Evita conversiones constantes y reduce errores en runtime.

**Formato `CAMPO: valor` en el archivo de persistencia**  
→ Legible en cualquier editor de texto sin herramientas adicionales. Diseñado para operadores sin perfil técnico.

**Detección de 4 casos de arranque en `cargarBitacora()`**  
→ Primera ejecución / recuperación con datos / archivo limpio / archivo inválido. El sistema informa al operador en cada caso.

**Sobreescritura total con `ofstream`**  
→ Garantiza que `bitacora.txt` siempre refleje el estado más reciente, sin acumulación de datos inconsistentes.

---

## 🖥️ Cómo compilar y ejecutar

```bash
# Con g++ directo
g++ -o sigconecta src/sigconecta.cpp

# Con Make
make

# Ejecutar
./sigconecta
```

> Requiere g++ con soporte C++11 o superior.

---

## 📂 Estructura del proyecto

```
sig-conecta/
├── src/
│   └── sigconecta.cpp       # Código fuente principal
├── samples/
│   └── bitacora_ejemplo.txt # Archivo de persistencia de muestra
├── docs/
│   └── informe_tecnico.pdf  # Informe de diseño e implementación
├── Makefile
└── README.md
```

---

## 📸 Vista del sistema

```
=== Iniciando SIG-Conecta v2.0 ===
[+] Sesion anterior recuperada: 3 incidente(s) cargados desde 'bitacora.txt'.

========================================
   SIG-CONECTA | San Pedro de la Montana
========================================
1. Consultar estado de equipos
2. Modificar estado de un equipo
3. Registrar incidente
4. Consultar historial (bitacora)
5. Guardar y salir
========================================
Elige una opcion:
```

---

## 🧠 Conceptos aplicados

| Concepto | Implementación |
|----------|---------------|
| Estructuras (`struct`) | `RegistroBitacora` con 4 campos tipados |
| Arreglos | `int equipos[20]` para estados |
| Persistencia de archivos | `ofstream` / `ifstream` con `<fstream>` |
| Modularización | 8 funciones con responsabilidad única |
| Sanitización de entradas | `cin.fail()` + `cin.clear()` + `cin.ignore()` |
| Flujo de control | `while(true)` + `switch` + `break` |

---

*Proyecto desarrollado en abril 2026 · UNRC Semestre 2*
