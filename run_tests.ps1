#!/usr/bin/pwsh

g++ test/test_reader.cpp datareader.cpp path_logic.cpp -o test/test_reader.exe

./test/test_reader.exe > test/read_output.txt

Remove-Item test/test_reader.exe

Compare-Object (Get-Content test/read_output.txt) (Get-Content test/read_output_ref.txt) > test/read_comparison.txt

if ((Get-Content "test/read_comparison.txt") -eq $Null) {
    Remove-Item test/read_comparison.txt
    Remove-Item test/read_output.txt
} else {
    "Unexpected result of node information reading test"
}

g++ test/test_points.cpp path_logic.cpp datareader.cpp -o test/test_points.exe

./test/test_points.exe

Remove-Item test/test_points.exe

g++ test/test_solver.cpp path_logic.cpp datareader.cpp -o test/test_solver.exe

./test/test_solver.exe > test/solver_output.txt

Remove-Item test/test_solver.exe