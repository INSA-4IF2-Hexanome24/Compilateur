# Mini-compilateur C (ifcc)

github: git@github.com:INSA-4IF2-Hexanome24/Compilateur.git

Ce projet contient un mini-compilateur pour un sous-ensemble du langage C. Il est développé en C++ avec ANTLR4 pour l'analyse syntaxique et utilise une visite en deux passes :

1. **Analyse sémantique et table des symboles** via `SymbolTableVisitor`.
2. **Génération de code assembleur x86_64** via `CodeGenVisitor`.


## Fonctionnalités implémentées

- Reconnaissance d'un programme `int main() { ... return expr; }`.
- Déclaration de variables (`int a;`, `int b = expr;`).
- Affectation (`x = expr;`).
- Expressions arithmétiques (`+`, `-`, `*`) avec précédence.
- Opérations bit-`or` (`|`), bit-`xor` (`^`), bit-`and` (`&`).
- Comparaisons (`==`, `!=`, `<`, `>`).
- Opérateur négation logique (`!`).
- Constantes entières et variables.
- Gestion simple de la pile, empilement et dépilement pour évaluer les opérations.
- Contrôles semantiques : variables non déclarées, instructions multiples, utilisation de variables non lues.


## Structure du code

```
compiler/
  ├── ifcc.g4               # Grammaire ANTLR du langage
  ├── generated/            # Fichiers produits par ANTLR (lexer, parser, base visitor, etc.)
  ├── SymbolTableVisitor.h/cpp   # détection et répartition des variables
  ├── CodeGenVisitor.h/cpp       # production du code assembleur
  ├── main.cpp              # programme principal (analyse + génération)
  ├── Makefile / config.mk  # scripts de construction
  └── prog/                 # sources de tests et exemples

ifcc-test.py                # automatisation des tests en Python
ifcc-test-output/           # résultats attendus des tests
README.md                   # ce document
```


### Points d'entrée principaux

- `main.cpp` : lit un fichier source, construit un arbre de syntaxe abstraite avec ANTLR, puis applique successivement `SymbolTableVisitor` et `CodeGenVisitor`.
- `SymbolTableVisitor::visit*` : récolte les déclarations, vérifie les erreurs semantiques, et calcule les décalages de pile.
- `CodeGenVisitor::visit*` : visite l'arbre pour mettre à jour symboles, évaluer les expressions et écrire le code assembleur vers `stdout`.


### Compilation et exécution

1. Générer le code ANTLR :
   ```sh
   cd compiler
   make
   ```
2. Utiliser `ifcc` sur un fichier `.c` :
   ```sh
   ./ifcc path/to/test.c > out.s # "> out.s" sert à enregistrer le code assembleur dans le fichier out.s
   gcc -o out out.s
   ./out
   ```


## Navigation dans le code

1. **Grammaire** : `ifcc.g4` définit la syntaxe du langage. Chaque régle correspond à une classe de contexte `ifccParser::...Context` utilisée par les visiteurs.

2. **Fichiers générés par ANTLR** : `generated/` contient le lexer, le parser et les classes de base des visiteurs. Ils servent de fondation aux visiteurs personnalisées.

3. **SymbolTableVisitor** (`SymbolTableVisitor.h/.cpp`) :
   - `visitDecl_stmt` : enregistre les variables et leurs décalages de pile.
   - `visitAssign_stmt` et `visitPrimary` : vérifient l'existence des variables et traquent les utilisations.
   - `visitProg` : diagnostic final et affichage de la table des symboles.

4. **CodeGenVisitor** (`CodeGenVisitor.h/.cpp`) :
   - `visitProg` : initialise la pile, appelle les enfants et termine par `ret`.
   - Les méthodes `visit...Expr` implémentent la génération d'instructions pour chaque niveau de précédence.
   - `visitDecl_stmt`, `visitAssign_stmt`, `visitReturn_stmt` traitent respectivement l'initialisation, l'affectation et le retour.
   - `pushRax`/`popRcx` sont des utilitaires internes pour sauvegarder la valeur courante pendant l'évaluations.


> **Conseil** : commencez par lire `ifcc.g4` pour comprendre la structure du langage, puis suivez les méthodes de visite correlées dans `SymbolTableVisitor` et `CodeGenVisitor`.


## Tests et validations

Le répertoire `testfiles/` contient des programmes C pour tester les différentes fonctionnalités implémentées par notre compilateur. Le script `ifcc-test.py` automatise l'exécution des cas de test et la vérification des résultats.

Pour une éxecution rapide depuis le répertoire ./compiler vous pouvez taper:
  make tests

## Présentation de l’hexanôme

### L’équipe projet est composée de sept membres :

Karim Madrane
Martín Vásquez
Josué Vega
Rida Bindech
Juan José Díaz
Ewan Garoux
Liam Le Touze
Organisation et répartition des rôles

  Afin d’assurer le bon déroulement du projet, certains rôles spécifiques ont été attribués :

    Juan José Díaz : Chef de projet
      Coordination globale du projet
      Organisation des tâches et suivi de l’avancement
    Ewan Garoux : Responsable Git
      Gestion du dépôt Git
      Supervision des branches et leur fusion.
    Liam Le Touze : Responsable de la mise en commun
      Intégration des différentes parties du projet
      Vérification de la cohérence globale du code

  Les autres membres de l’équipe ont principalement occupé un rôle de développeurs :
  
    Karim Madrane
    Martín Vásquez
    Josué Vega
    Rida Bindech

Ils ont contribué au développement des différentes fonctionnalités du compilateur, à l’implémentation des visiteurs, ainsi qu’à la réalisation et à la validation des tests.

Remarque: Bien que des rôles principaux aient été définis, le travail a été réalisé de manière collaborative. Chaque membre a pu intervenir sur plusieurs aspects du projet (développement, tests, débogage), selon les besoins.
