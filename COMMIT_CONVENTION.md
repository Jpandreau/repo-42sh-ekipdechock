# Commit Convention

## Format

```
[TAG] (scope) Imperative short message
```

- **TAG** : obligatoire
- **scope** : optionnel mais fortement conseillé (nom du module/fichier concerné)
- **message** : impératif, anglais, 50 caractères max, sans point final

```
[ADD] (parser) Handle quoted string tokens
[FIX] (builtin/cd) Resolve OLDPWD not updated
[CODINGSTYLE] (lexer) Rename ambiguous variable names
```

---

## Tags

### Principaux

| Tag | Usage |
|---|---|
| `[ADD]` | Nouvelle fonctionnalité ou nouveau fichier |
| `[MODIF]` | Modification d'un existant sans changer son rôle |
| `[FIX]` | Correction de bug |
| `[CODINGSTYLE]` | Norme de code uniquement — zéro logique changée |
| `[REM]` | Suppression de commentaire ou de code commenté |
| `[DEL]` | Suppression de fichier ou de feature entière |

### Supplémentaires

| Tag | Usage |
|---|---|
| `[REFACTO]` | Restructuration sans changement de comportement |
| `[TEST]` | Ajout ou modification de tests |
| `[DOCS]` | README, commentaires de doc, Makefile |
| `[MERGE]` | Merge de branche |
| `[WIP]` | Commit intermédiaire — **interdit sur `main` et `dev`** |

---

## Règles

1. Un commit = une chose — ne pas mélanger les tags
2. Message impératif : `Add`, pas `Added` ni `Adding`
3. 50 caractères max pour le message
4. Pas de point final
5. `[WIP]` interdit sur `main` et `dev`
