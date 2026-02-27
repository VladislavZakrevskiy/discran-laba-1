#!/usr/bin/env bash
# Я не захотел Google Test, потому что 
# 1. честно лень было, 
# 2. он тут избыточек, я просто файлики наделал и кладу в стандартный ввод программе и так в цикле 
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
SOLUTION_BIN="${ROOT_DIR}/solution"
CASES_DIR="${ROOT_DIR}/tests/cases"

if [[ ! -x "${SOLUTION_BIN}" ]]; then
    echo "Binary ${SOLUTION_BIN} not found. Run 'make' first."
    exit 1
fi

if [[ ! -d "${CASES_DIR}" ]]; then
    echo "Cases directory ${CASES_DIR} not found."
    exit 1
fi

found_any=0

for input_file in "${CASES_DIR}"/*.in; do
    [[ -e "${input_file}" ]] || continue
    found_any=1

    name="$(basename "${input_file}" .in)"
    expected_file="${CASES_DIR}/${name}.out"
    actual_file="$(mktemp)"

    if [[ ! -f "${expected_file}" ]]; then
        echo "Missing expected file for test '${name}': ${expected_file}"
        rm -f "${actual_file}"
        exit 1
    fi

    "${SOLUTION_BIN}" <"${input_file}" >"${actual_file}"

    if ! diff -u "${expected_file}" "${actual_file}"; then
        echo "Test failed: ${name}"
        rm -f "${actual_file}"
        exit 1
    fi

    echo "Test passed: ${name}"
    rm -f "${actual_file}"
done

if [[ "${found_any}" -eq 0 ]]; then
    echo "No test cases found in ${CASES_DIR}."
    exit 1
fi

echo "All tests passed."
