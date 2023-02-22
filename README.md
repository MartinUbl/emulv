# Modulární emulátor platformy RISC-V
## Popis zadání:

Pro potřeby výuky vznikla potřeba mít emulátor hardwarové platformy RISC-V. V
předmětu KIV/OS momentálně používáme hardware založený na ARM procesorech,
nicméně budoucnost vidíme v použití platformy RISC-V. Jde o open-source
platformu, která je důkladně zdokumentovaná a existuje k ní spousty materiálů jak ve
formě výukových prostředků, tak např. již hotových knihoven pro emulaci instrukční
sady.

V rámci tohoto zadání byste vytvořili emulátor platformy RISC-V (základní podoby
jeho ISA), který by zahrnoval i uživatelské rozhraní. V tomto rozhraní by byl vidět
aktuální tok instrukcí, obsah registrů a byla by možnost prohlížet i emulovanou
paměť. Mimo to by bylo velice vhodné umožnit vkládat do kódu breakpointy a kód
umět krokovat. Emulátor by měl být i dostatečně modulární - můžeme například chtít
“připojit” modul na určité emulované piny procesoru (např. LED, spínač/přepínač, v
budoucnu i displej nebo jiné složitější periferie), přičemž emulátor by pro každý z
těchto modulů uměl otevřít podokno s emulovanou, potenciálně ovladatelnou
podobou této periferie. Emulátor by měl být multiplatformní (v současnosti postačuje
Debian GNU/Linux a MS Windows 10). Preferovaným, nicméně ne vyžadovaným
implementačním jazykem je C++.

---

> Předpokládaná náročnost: 240 člověkohodin

> Kontakt: Martin Úbl, ublm@kiv.zcu.cz

---
