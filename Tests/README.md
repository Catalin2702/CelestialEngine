# CelestialEngine Tests

Questa directory contiene tutti i test per il progetto CelestialEngine.

## Struttura

```
Tests/
├── Engine/			# Test per il motore
│   ├── Core/		# Test per CE_Core
│   ├── Define/		# Test per CE_Define (macro compilation tests)
│   ├── Events/		# Test per CE_Events
│   └── Tools/		# Test per CE_Tools
└── Editor/			# Test per l'editor (futuro)
```

## Eseguire i Test

### Tutti i test del progetto

```bash
cd .debug
ctest --output-on-failure
# oppure
cmake --build . --target RunAllTests
```

### Solo i test di Engine

```bash
cd .debug
ctest -L Engine --output-on-failure
# oppure
cmake --build . --target RunEngineTests
```

### Test specifici per modulo

```bash
cd .debug
ctest -L Events --output-on-failure  # Solo test Events
ctest -L Tools --output-on-failure   # Solo test Tools
ctest -L Core --output-on-failure	# Solo test Core
ctest -L Define --output-on-failure  # Solo test Define
```

### Test con output verbose

```bash
cd .debug
ctest --verbose
```

### Solo test falliti

```bash
cd .debug
ctest --rerun-failed --output-on-failure
```

## Label

I test sono organizzati con i seguenti label:

- **Engine**: Tutti i test del motore
- **Core**: Test del core (quando implementati)
- **Define**: Test delle macro (compilation tests)
- **Events**: Test del sistema di eventi
- **Tools**: Test degli strumenti

## Aggiungere Nuovi Test

1. Crea una nuova directory in `Tests/Engine/` (o `Tests/Editor/` per test dell'editor)
2. Crea un file `CMakeLists.txt` nella nuova directory
3. Definisci l'eseguibile di test e aggiungi il test con i label appropriati:

```cmake
set(SOURCES
	MyModuleTests.cpp
)

add_executable(CE_TestsMyModule ${SOURCES})

target_link_libraries(CE_TestsMyModule PRIVATE
	CE_Config
	CE_MyModule
	GTest::gtest
	GTest::gtest_main
)

add_test(NAME MyModuleTests
	COMMAND CE_TestsMyModule
	WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}/Binaries/$<CONFIG>
)

set_tests_properties(MyModuleTests PROPERTIES
	LABELS "Engine;MyModule"
)
```

4. Aggiungi la subdirectory nel CMakeLists.txt padre: `add_subdirectory(MyModule)`

