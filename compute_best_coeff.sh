


for i in $(seq 0.0 0.1 1.0)
do
    make client_new_heuristic.so -e COEFF=$i
    echo ""
    echo "########################################" >> coeff_results.txt
    echo "COEFF = $i" >> coeff_results.txt
    ./install/arena -r 30 ./install/client_power_heuristic.so ./install/client_new_heuristic.so >> coeff_results.txt
    echo ""
    echo ""
    ./install/arena -r 30 ./install/client_new_heuristic.so ./install/client_power_heuristic.so >> coeff_results.txt
done









