# CelestialEngine Tests

Questa directory contiene tutti i test per il progetto CelestialEngine.

## Struttura

L'albero dei test rispecchia quello di `Engine/Modules/`: ogni modulo del motore ha la sua directory con lo stesso
percorso interno dei sorgenti (`src/<Modulo>/<Area>/Platforms/<OS>/<API>/`).

```
Tests/
└── Engine/
    └── Modules/
        ├── Core/       # Test per CE_Core (Input, Layers, Hub)
        ├── Define/     # Test per CE_Define (macro compilation tests)
        ├── Events/     # Test per CE_Events
        ├── Native/     # Test per CE_Native (bridge ImGui, dispatcher AppKit/MetalKit)
        ├── Tools/      # Test per CE_Tools (logging)
        ├── Types/      # Test per CE_Types (key code, render, window, build)
        └── Utility/    # Test per CE_Utility (CMD, Config, Delegate, FileSystem, ImGui, String, Time)
```

## Eseguire i Test

I comandi vanno lanciati dalla directory di build (per esempio `.build/debug-apple-clang`).

### Tutti i test del progetto

```bash
cd .build/debug-apple-clang
ctest --output-on-failure
# oppure
cmake --build . --target RunAllTests
```

### Solo i test di Engine

```bash
ctest -L Engine --output-on-failure
# oppure
cmake --build . --target RunEngineTests
```

### Test specifici per modulo

```bash
ctest -L Core --output-on-failure     # Solo test Core
ctest -L Define --output-on-failure   # Solo test Define
ctest -L Events --output-on-failure   # Solo test Events
ctest -L Native --output-on-failure   # Solo test Native
ctest -L Tools --output-on-failure    # Solo test Tools
ctest -L Types --output-on-failure    # Solo test Types
ctest -L Utility --output-on-failure  # Solo test Utility
```

### Test con output verbose

```bash
ctest --verbose
```

### Solo test falliti

```bash
ctest --rerun-failed --output-on-failure
```

### Un singolo caso di test

Gli eseguibili sono copiati in `Binaries/Last/` e accettano i filtri di GoogleTest:

```bash
./Binaries/Last/CE_TestsCore --gtest_filter='InputTest.*'
```

## Eseguibili e Label

| Eseguibile         | Test CTest    | Label              |
|--------------------|---------------|--------------------|
| `CE_TestsCore`     | `CoreTests`   | `Engine;Core`      |
| `CE_TestsDefine`   | `DefineTests` | `Engine;Define`    |
| `CE_TestsEvents`   | `EventsTests` | `Engine;Events`    |
| `CE_TestsNative`   | `NativeTests` | `Engine;Native`    |
| `CE_TestsTools`    | `ToolsTests`  | `Engine;Tools`     |
| `CE_TestsTypes`    | `TypesTests`  | `Engine;Types`     |
| `CE_TestsUtility`  | `UtilityTests`| `Engine;Utility`   |

## Cosa viene testato (e cosa no)

I test sono **headless**: nessuno di essi apre una finestra, inizializza GLFW o crea un device Metal, così possono
girare anche senza display. Questo determina cosa è coperto:

- **Core**: lo stato dell'input (guidato dagli eventi dell'hub, non dal polling del sistema operativo), lo stack
  dei layer e la traduzione degli eventi degli hub (`GlfwEventHubDispatcher`, `CocoaEventHubDispatcher`).
- **Native**: il bridge ImGui e i dispatcher AppKit/MetalKit, verificati sull'inoltro dei callback nativi (i
  puntatori `NS::Event` / `MTK::View` non vengono mai dereferenziati dai dispatcher, quindi bastano indirizzi finti).
- **Events / Types / Define / Tools / Utility**: interamente coperti, non dipendono dalla piattaforma.

Non sono coperti da test automatici i componenti che richiedono una finestra o una GPU reale: `GlfwWindow`,
`CocoaWindow`, `GlfwApplication`, `CocoaApplication`, i context (`OpenGlContext`, `MetalContext`), gli shader e i
layer ImGui concreti. Le parti *logiche* di quei percorsi (conversione dei codici tasto, creazione e instradamento
degli eventi, gestione dello stato) sono però testate attraverso gli hub e i moduli Types/Native.

## Aggiungere Nuovi Test

1. Crea il file di test in `Tests/Engine/Modules/<Modulo>/src/...`, rispecchiando il percorso del sorgente testato.
2. Aggiungilo alla lista `SOURCES` del `CMakeLists.txt` del modulo (i file specifici per macOS vanno nel blocco
   `$<$<PLATFORM_ID:Darwin>:...>`).
3. Se si tratta di un modulo nuovo, crea il suo `CMakeLists.txt`:

```cmake
set(SOURCES
	src/MyModuleTests.cpp
)

add_executable(CE_TestsMyModule ${SOURCES})

target_link_libraries(CE_TestsMyModule PRIVATE
	CE_Config
	CE_TestConfig
	CE_MyModule
	GTest::gtest_main
)

add_test(NAME MyModuleTests
	COMMAND CE_TestsMyModule
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>
)

set_tests_properties(MyModuleTests PROPERTIES
	LABELS "Engine;MyModule"
)

ce_copy_to_last(CE_TestsMyModule)
```

4. Aggiungi la subdirectory in `Tests/Engine/CMakeLists.txt`: `add_subdirectory(Modules/MyModule)`.
