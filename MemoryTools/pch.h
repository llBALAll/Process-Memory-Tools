// pch.h: esse é um arquivo de cabeçalho pré-compilado.
// Os arquivos listados a seguir são compilados somente uma vez, melhorando o desempenho dos builds futuros.
// Isso também afeta o desempenho do IntelliSense, incluindo a conclusão de código e muitos recursos de navegação de código.
// No entanto, os arquivos listados aqui serão TODOS recompilados se qualquer um deles for atualizado entre builds.
// Não adicione aqui arquivos que você atualizará com frequência, pois isso anula a vantagem de desempenho.

#ifndef PCH_H
#define PCH_H

#define WIN32_LEAN_AND_MEAN             // Excluir itens raramente utilizados dos cabeçalhos do Windows

// adicione os cabeçalhos que você deseja pré-compilar aqui
#include "framework.h"

// Incluedes Padrões para Funcionamento correto da Lib
#include <stdio.h>
#include <Windows.h>
#include <tlhelp32.h>
#include <thread>
#include <tchar.h>
#include <String>
#include <vector>
#include <iostream>

#endif //PCH_H

