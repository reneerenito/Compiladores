#!/bin/sh
# Corre todas las pruebas de pruebas/casos.txt y dice si salió lo esperado.
# Uso:  make test    (o  sh pruebas/correr_pruebas.sh  desde la carpeta del proyecto)

cd "$(dirname "$0")/.." || exit 1
bien=0
mal=0

while IFS= read -r linea; do
    case "$linea" in
        ''|'#'*) continue ;;
    esac
    esperado=${linea%% *}
    codigo=$(printf '%s\n' "${linea#* }" | sed 's/^ *//')

    if ./parser "$codigo" > /dev/null; then obtenido=ACEPTA; else obtenido=RECHAZA; fi

    if [ "$obtenido" = "$esperado" ]; then
        bien=$((bien + 1)); marca="ok   "
    else
        mal=$((mal + 1)); marca="FALLO"
    fi
    printf '[%s] %-8s %s\n' "$marca" "$obtenido" "$codigo"
done < pruebas/casos.txt

echo
echo "$bien bien, $mal mal"
[ "$mal" -eq 0 ]
