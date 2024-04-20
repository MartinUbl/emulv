# Modulární emulátor platformy RISC-V pro výukové účely (BP 2023/24)

# Zadání

Bakalářská práce v oborech Informatika (Bc), Výpočetní technika (Bc), Informační systémy (Bc).

### Modulární emulátor platformy RISC-V pro výukové účely

1. Seznamte se s architekturou RISC-V a nejrozšířenějšími implementacemi její instrukční sady
2. Analyzujte dostupná řešení pro emulaci a ladění programů pro tuto architekturu
3. Navrhněte nástroj, který umožní emulovat platformu RISC-V s možností definovat a konfigurovat periferie odděleně od jeho jádra a provádět
   úkony související s laděním
4. Implementujte tento nástroj jako modulární, umožněte vytvářet periferie definované v dynamicky linkovaných knihovnách
5. Otestujte řešení na sadě standardních úloh a zhodnoťte dosažené výsledky

| Téma vypsal                | Vypsáno pro akademický rok | Zadáno komu |
|----------------------------|----------------------------|-------------|
| *Ing. Martin Úbl* (UN 332) | 2023/2024 (dne 2023-04-11) | Jonáš Dufek |


# Závislosti projektu

### Knihovny nainstalové v OS potřebné pro kompilaci

| Název              | Účel                    |
|--------------------|-------------------------|
| Doxygen + GraphViz | Generování dokumentace  |
| Qt6.6              | Knihovna pro tvorbu GUI |

### Následující knihovny jsou automaticky staženy pomocí CMake:

| Název               | Odkaz                                                                                                      |
|---------------------|------------------------------------------------------------------------------------------------------------|
| libriscv            | [https://github.com/fwsGonzo/libriscv](https://github.com/fwsGonzo/libriscv)                               |
| doxygen-awesome-css | [https://github.com/jothepro/doxygen-awesome-css](https://github.com/jothepro/doxygen-awesome-css)         |
| JSON for Modern C++ | [https://github.com/nlohmann/json](https://github.com/nlohmann/json)                                       |
| riscv-disassembler  | [https://github.com/michaeljclark/riscv-disassembler](https://github.com/michaeljclark/riscv-disassembler) |
| spdlog              | [https://github.com/gabime/spdlog](https://github.com/gabime/spdlog)                                       |
| GoogleTest          | [https://github.com/google/googletest](https://github.com/google/googletest)                               |
| qwindowkit          | [https://github.com/stdware/qwindowkit](https://github.com/stdware/qwindowkit)                             |
