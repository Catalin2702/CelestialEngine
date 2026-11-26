# CelestialEngine Tests

Questa directory contiene i test dell'engine. Sta dentro `Engine/` perché i test appartengono all'engine: lo seguono
se viene incluso da un altro progetto, e si disattivano con `-DCE_BUILD_TESTS=OFF`.

## Struttura

L'albero dei test rispecchia quello di `Engine/Source/Runtime/`: ogni modulo del motore ha la sua directory con lo stesso
percorso interno dei sorgenti (`src/<Modulo>/<Area>/Platforms/<OS>/<API>/`).

```
Engine/Tests/
├── CMake/          # Test delle funzioni CMake (script cmake -P): ce_collect_application_content
├── Programs/
│   └── ShaderCompiler/  # Test di CE_ShaderCompiler, eseguito come lo esegue la build (data/: shader di prova)
└── Runtime/
    ├── Core/       # Test per CE_Core (CMD, Config, Input, Layers e helper ImGui, bridge ImGui-Metal, Hub e codici tasto GLFW/Cocoa,
    │               #   camera, ForwardRenderer con un RHI finto)
    ├── Define/     # Test per CE_Define (BIND_FN_*)
    ├── Events/     # Test per CE_Events (eventi e macro EVENT_CLASS_*)
    ├── IO/         # Test per CE_IO (VirtualPath: Parse, Compose, Mount; FileSystem su cartelle temporanee)
    ├── Native/     # Test per CE_Native (dispatcher AppKit/MetalKit)
    ├── RHI/        # Test per CE_RHI (header, BufferLayout, descrittori, Viewport, conversioni OpenGL, riflessione degli shader)
    ├── Tools/      # Test per CE_Tools (logging)
    ├── Types/      # Test per CE_Types (BIT, key code, render, window, build) - niente conversioni verso i backend
    └── Utility/    # Test per CE_Utility (Delegate, Dispatcher, String, Time; che l'umbrella non porti dentro ImGui o Core)
```

## Eseguire i Test

I comandi vanno lanciati dalla directory di build (per esempio `.exclude/build/clang`).

### Tutti i test del progetto

```bash
cd .exclude/build/clang
ctest -C Debug --output-on-failure
# oppure
cmake --build . --target RunAllTests
```

### Solo i test di Engine

```bash
ctest -C Debug -L Engine --output-on-failure
# oppure
cmake --build . --target RunEngineTests
```

### Test specifici per modulo

```bash
ctest -C Debug -L Core --output-on-failure     # Solo test Core
ctest -C Debug -L Define --output-on-failure   # Solo test Define
ctest -C Debug -L Events --output-on-failure   # Solo test Events
ctest -C Debug -L Native --output-on-failure   # Solo test Native
ctest -C Debug -L RHI --output-on-failure      # Solo test RHI
ctest -C Debug -L Tools --output-on-failure    # Solo test Tools
ctest -C Debug -L Types --output-on-failure    # Solo test Types
ctest -C Debug -L Utility --output-on-failure  # Solo test Utility
ctest -C Debug -L CMake --output-on-failure    # Solo test delle funzioni CMake
ctest -C Debug -L ShaderCompiler --output-on-failure  # Solo test di CE_ShaderCompiler
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

Gli eseguibili stanno nella cartella di build, in `Out/<config>/Tests/`, e accettano i filtri di GoogleTest:

```bash
./.exclude/build/clang/Out/Debug/Tests/CE_TestsCore --gtest_filter='InputTest.*'
```

## Eseguibili e Label

| Eseguibile         | Test CTest    | Label              |
|--------------------|---------------|--------------------|
| `CE_TestsCore`     | `CoreTests`   | `Engine;Core`      |
| `CE_TestsDefine`   | `DefineTests` | `Engine;Define`    |
| `CE_TestsEvents`   | `EventsTests` | `Engine;Events`    |
| `CE_TestsIO`       | `IOTests`     | `Engine;IO`        |
| `CE_TestsNative`   | `NativeTests` | `Engine;Native`    |
| `CE_TestsRHI`      | `RHITests`    | `Engine;RHI`       |
| `CE_TestsTools`    | `ToolsTests`  | `Engine;Tools`     |
| `CE_TestsTypes`    | `TypesTests`  | `Engine;Types`     |
| `CE_TestsUtility`  | `UtilityTests`| `Engine;Utility`   |
| `cmake -P`         | `ApplicationContentTests` | `Engine;CMake` |
| `CE_ShaderCompiler`| `ShaderCompiler.ValidShader`, `.BrokenShader`, `.UnknownStage` | `Engine;ShaderCompiler` |

## Cosa viene testato (e cosa no)

I test sono **headless**: nessuno di essi apre una finestra, inizializza GLFW o crea un device Metal, così possono
girare anche senza display. Questo determina cosa è coperto:

- **Core**: il parsing della riga di comando e `Config`, lo stato dell'input (guidato dagli eventi dell'hub, non
  dal polling del sistema operativo), lo stack dei layer, la conversione dei tasti verso ImGui e il bridge verso il
  backend Metal di ImGui (solo che le funzioni esistano e linkino), la traduzione degli eventi degli hub (`GlfwEventHubDispatcher`, `CocoaEventHubDispatcher`), il
  `ForwardRenderer` su device e swapchain finti, e che le finestre siano superfici dell'RHI (`RHI::I_Surface`).
- **RHI**: le parti che non toccano la GPU - `BufferLayout`, descrittori di pass e pipeline, `Viewport`, conversioni
  verso OpenGL - e che ogni umbrella dell'RHI compili da solo, senza header di Core. `CE_TestsRHI` linka `CE_RHI` e
  non `CE_Core`: se l'RHI tornasse a dipendere da Core, il test non linkerebbe più. Legge anche i file generati dalla
  build degli shader (`CE_SHADERS_GENERATED_DIR`): la riflessione deve avere i blocchi di costanti agli slot e con le
  dimensioni di `ShaderConstants.hpp`, gli ingressi del vertex shader nell'ordine di `BufferLayout`, le uscite del
  vertex uguali agli ingressi del fragment; il GLSL deve essere 4.10 senza `binding`, l'MSL deve avere gli entry point
  e gli indici `buffer(1)`, `buffer(2)`, `texture(0)`.
- **CMake**: `ce_collect_application_content` su una cartella finta (file annidati, ordine, `.DS_Store` e `.git/`
  esclusi), eseguita come script senza configurare un'applicazione.
- **ShaderCompiler**: il programma su uno shader valido (tutte le uscite), uno con un errore (il messaggio indica
  `file:riga`) e un'estensione sconosciuta.
- **Native**: i dispatcher AppKit/MetalKit, verificati sull'inoltro dei callback nativi (i
  puntatori `NS::Event` / `MTK::View` non vengono mai dereferenziati dai dispatcher, quindi bastano indirizzi finti).
- **Events / Types / Define / Tools / Utility**: interamente coperti, non dipendono dalla piattaforma.
- **IO**: `VirtualPath` senza disco, `FileSystem` su una cartella temporanea montata per ogni test. `Initialize` non
  viene chiamato: monterebbe le cartelle vere dell'utente (`~/Library/Application Support`, `~/Library/Caches`).

Non sono coperti da test automatici i componenti che richiedono una finestra o una GPU reale: `GlfwWindow`,
`CocoaWindow`, `Application`, le piattaforme (`GlfwPlatform`, `CocoaPlatform`), i device e le swapchain
(`OpenGlGraphicDevice`, `MetalGraphicDevice`, `OpenGlSwapchain`, `MetalSwapchain`), l'esecuzione degli shader sulla
GPU e i layer ImGui concreti. Le parti *logiche* di quei percorsi (conversione dei codici tasto, creazione e instradamento
degli eventi, gestione dello stato) sono però testate attraverso gli hub e i moduli Types/Native.

## Aggiungere Nuovi Test

1. Crea il file di test in `Engine/Tests/Runtime/<Modulo>/src/...`, rispecchiando il percorso del sorgente testato.
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
	WORKING_DIRECTORY $<TARGET_FILE_DIR:CE_TestsMyModule>
)

set_tests_properties(MyModuleTests PROPERTIES
	LABELS "Engine;MyModule"
)

# Out/<config>/Tests/, and the module libraries found on every platform
ce_set_output_subdirectory(CE_TestsMyModule Tests)
ce_enable_dll_bootstrap(CE_TestsMyModule)
ce_set_dll_search_rpath(CE_TestsMyModule "../DLL")
ce_embed_apple_modules_beside(CE_TestsMyModule Tests)
```

4. Aggiungi la subdirectory in `Engine/Tests/CMakeLists.txt`: `add_subdirectory(Runtime/MyModule)`.
