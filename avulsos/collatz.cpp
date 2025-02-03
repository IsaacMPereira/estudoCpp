/*
 *  C++ program to analise the Collatz Conjecture
 *  Compile: $ g++ -Wall collatz.cpp -o collatz.x
 *  Run: $ ./collatz.x z
 *
 *  The file dados1_collatz.txt have a desired quantity of numbers with the corresponding quantity numbers generated by the Collatz Conjecture it self.
 *  The file dados2_collatz.txt have a desired quantity of numbers with all the numbers generated by the Colatz Conjecture listed discreetly.
 *  The file dados3_collatz.txt the values of count 
 * */

#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <fstream>
#include <map>
#include <string>
#include <vector>
#include <algorithm>
#include <set>

using namespace std;

int main(int argc, char *argv[2]){ //Quando tiver mais memória, analisar para um i = 500.000
  if(argc != 2){
    cerr << "Error! Run: $ ./collatz.x i" << endl;
    exit(1);
  }
  if(atoi(argv[1]) > 20000){
    cout << "Ta maluco, cara? KKKKKK Precisa de muita memoria!" << endl;
    exit(1);
  }

  FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
  if(!gnuplotPipe){
    cerr << "Error on openning Gnuplot pipe" << endl;
    exit(1);
  }

  int count=0;
  vector<int> v_count, v_dif;
  set<int> s_count, s_dif;

  ofstream file1_escrita("dados1_collatz.txt");
  ofstream file2_escrita("dados2_collatz.txt"); 
  ofstream file3_escrita("dados3_collatz.txt");
  ofstream file4_escrita("dados4_collatz.txt");

  if(file1_escrita.is_open() and file2_escrita.is_open()){
    for(int i=1; i<=atoi(argv[1]); i++){
      int aux = i;
      file2_escrita << "-> "<< i << ":";
      while(aux){
        if(aux == 1){
          break;
        }else if(aux%2 == 0){
          aux = aux/2;
          count++;
          file2_escrita << " " << aux;
        }else if(aux%2 != 0){
          aux = 3*aux + 1;
          count++;
          file2_escrita << " " << aux;
        }
      }
      file1_escrita << i << "\t" << count << endl;
      file2_escrita << endl;
      v_count.push_back(count);
      s_count.insert(count);
      count = 0;
    }
    file1_escrita.close();
    file2_escrita.close();
  }else{
    cerr << "Error on openning one of the files" << endl;
    exit(1); 
  }

  sort(v_count.begin(), v_count.end());

  map<int, int> histograma_count;
  for(auto e : v_count)
    histograma_count[e]++;

  cout << "Histograma de count:" << endl;
  for(const auto& e : histograma_count){
    cout << e.first << ":" << e.second << endl;
    file3_escrita << e.first << "\t" << e.second << endl;
  }
  file3_escrita.close();

  cout << endl << "Set de count:" << endl;
  for(auto e : s_count)
    cout << e << endl;

  cout << endl << "O set de count é sucessivamente perfeito?" << endl;
  int i=0;
  for(auto e : s_count){
    if(e != i){
      int dif = e-i;
      v_dif.push_back(dif);
      s_dif.insert(dif);
      cout << "Não! e:" << e << " i:" << i << " dif: " << dif << endl;
    }
    i++;
  }

  map<int, int> histograma_dif;
  for(auto e : v_dif)
    histograma_dif[e]++;

  cout << endl << "Histograma de dif:" << endl;
  for(const auto& e : histograma_dif){
    cout << e.first << ":" << e.second << endl;
    file4_escrita << e.first << "\t" << e.second << endl;
  }
  file4_escrita.close();

  cout << endl << "Set de dif:" << endl;
  for(auto e : s_dif)
    cout << e << endl;

  //system("open dados1_collatz.txt");
  //system("open dados2_collatz.txt");
  //system("open dados3_collatz.txt");

  //fprintf(gnuplotPipe, "set terminal wxt size 1700,850\n"); //MultiPlot
  //fprintf(gnuplotPipe, "set multiplot layout 1,3 title 'Análise da Conjectura de Collatz'\n"); //Multiplot
  fprintf(gnuplotPipe, "set terminal pdf size 10,8\n"); //Output: pdf
  fprintf(gnuplotPipe, "set output 'grafico3_collatz.pdf'\n"); //Output: pdf
  fprintf(gnuplotPipe, "set xlabel 'Números'\n");
  fprintf(gnuplotPipe, "set ylabel 'Quantidade de números gerados'\n");
  fprintf(gnuplotPipe, "plot 'dados1_collatz.txt' u 1:2 w p pt 7 lc rgb 'red' notitle\n");
  fprintf(gnuplotPipe, "set xlabel 'Count'\n");
  fprintf(gnuplotPipe, "set ylabel 'Quantidade de repetições de count'\n");
  fprintf(gnuplotPipe, "plot 'dados3_collatz.txt' u 1:2 w lines lc rgb 'blue' notitle\n");
  fprintf(gnuplotPipe, "set xlabel 'Diferenças'\n");
  fprintf(gnuplotPipe, "set ylabel 'Quantidades de repetições de diferenças'\n");
  fprintf(gnuplotPipe, "set style fill solid 1.0\n");
  fprintf(gnuplotPipe, "set boxwidth 0.8\n");
  fprintf(gnuplotPipe, "set style line 1 lc rgb 'orange'\n");
  fprintf(gnuplotPipe, "plot 'dados4_collatz.txt' u 1:2 w boxes ls 1 notitle\n");
  fprintf(gnuplotPipe, "set output\n"); //Output: pdf
  //fprintf(gnuplotPipe, "unset multiplot\n"); //Multiplot

  pclose(gnuplotPipe);

  cout << endl;
  return 0;
}
