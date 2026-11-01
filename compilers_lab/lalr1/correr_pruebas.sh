#!/usr/bin/env bash

set -u

make parser || exit 1

total=0
correctas=0
fallidas=0

while IFS='|' read -r esperado entrada || [[ -n "$esperado" ]]; do

    [[ -z "$esperado" || "$esperado" == \#* ]] && continue

    total=$((total + 1))

    salida=$(./parser "$entrada" 2>&1)
    resultado=$?

    if [[ "$resultado" -eq "$esperado" ]]; then
        echo "[OK] Caso $total: $entrada"
        correctas=$((correctas + 1))
    else
        echo "[ERROR] Caso $total: $entrada"
        echo "  Esperado: $esperado"
        echo "  Obtenido: $resultado"
        echo "$salida"
        fallidas=$((fallidas + 1))
    fi

done < pruebas/casos.txt

echo
echo "Total: $total"
echo "Correctas: $correctas"
echo "Fallidas: $fallidas"

if [[ "$fallidas" -gt 0 ]]; then
    exit 1
fi

exit 0