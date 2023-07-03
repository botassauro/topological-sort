#include <iostream>
#include <iterator>
#include <algorithm>
#include <vector>
#include <set>
#include <cstring>
#include <graphviz/cgraph.h>

using namespace std;

#define SIZE 100

int t = 0;

/* struct que representa um vértice do grafo */
typedef struct Vertice {
    char nodo[SIZE];
    char vizinho_char[SIZE];
    int index;
    unsigned int estado;
    vector<int> lista_vizinhos;
    int pre;
    int pos;

} Vertice;

std::vector<Vertice*> ordenacao;

bool operator<(const Vertice &v1, const Vertice &v2) {
    return v1.nodo < v2.nodo;
}

/* função de comparação para funçao sort */
bool compare_v(Vertice p1, Vertice p2) {
    return (p1.nodo[0] < p2.nodo[0]);
}

/* retorna o índice de um vértice no vector v dado v->nodo */
int retornaIndex(vector<Vertice> v, char c[]) {

    vector<Vertice>::iterator it;
    for (it = v.begin(); it != v.end(); ++it) {
        //printf("teste: %c\n", it->nodo);
        if (strcmp(it->nodo, c) == 0) {
            
            return it->index;
        }
    }

    return -1;

}

/* função de busca em profundidade com cálculo de pré e pós ordem */
void ordenaFunc(vector<Vertice>* vertices, Vertice* v) {

    v->estado = 1;
    t++;
    v->pre = t;

    vector<int>::iterator it;
    int index;

    for (int i = 0; i < (v->lista_vizinhos.size()); i++) {
        index = v->lista_vizinhos.at(i);
        
        if (vertices->at(index).estado == 0)
            ordenaFunc(vertices, &vertices->at(index));
    }
    ordenacao.push_back(v);
    v->estado = 2;
    t++;
    v->pos = t;
}

/* função envelope da busca em profundidade, no caso de grafo com multiplas componentes */
void ordenaEnvelope(vector<Vertice>* vertices) {

    vector<Vertice>::iterator it;

    for (int i = 0; i < vertices->size(); i++) {
        if (vertices->at(i).estado == 0)
            ordenaFunc(vertices, &vertices->at(i));    
    }
}

/* verifica se há um arco de retorno de grafo */
bool temArcoRetorno(vector<Vertice> vertices_grafo) {

    vector<int>::iterator it;

    // p/ cada vertice u do grafo G
    for (int i = 0; i < vertices_grafo.size(); i++) {

        // p/ cada vizinho de saída v de u
        for (it = vertices_grafo.at(i).lista_vizinhos.begin(); it != vertices_grafo.at(i).lista_vizinhos.end(); ++it) {
            
            // v.pre < u.pre < u.pos < v.pos.
            if (vertices_grafo.at(*it).pre < vertices_grafo.at(i).pre &&
            vertices_grafo.at(*it).pre < vertices_grafo.at(*it).pos &&
            vertices_grafo.at(i).pos < vertices_grafo.at(*it).pos) {
                return true;
            }
        }
    }

    return false;
}

int main() { 

    char str[SIZE];

    vector<Vertice> arestas;
    vector<Vertice> vertices_grafo;
    set<string> set_v;
    int aux, aux2;
    int j = 0;

    Agraph_t *graph;
    Agnode_t *node;
    Agedge_t *edge;

    // lê o grafo do arquivo .dot
    graph = agread(stdin, 0);

    if (!graph) {
        fprintf(stderr, "Erro ao ler arquivo.\n");
        return 1;
    }

    if (agisdirected(graph) == false) {
        fprintf(stderr, "Erro: grafo fornecido não é um GDA.\n");
        return 1;
    }

    // itera sobre nodos do grafo
    for (node = agfstnode(graph); node; node = agnxtnode(graph, node)) {
        // pega os nodos que formam uma aresta
        for (edge = agfstout(graph, node); edge; edge = agnxtout(graph, edge)) {
            Agnode_t *target = aghead(edge);
            Vertice p;
            strncpy(p.nodo, agnameof(node), sizeof(agnameof(node)));
            strncpy(p.vizinho_char, agnameof(target), sizeof(agnameof(target)));
            arestas.push_back(p);
            //printf("%s -> %s\n", agnameof(node), agnameof(target));
        }
    }

    // lista de arcos de saida u -> v ordenados por u
    sort(arestas.begin(), arestas.end(), compare_v);
    
    vector<Vertice>::iterator it;

    // aqui TODOS os vertices do grafo
    // guardar em set evita repetição
    for (it = arestas.begin(); it != arestas.end(); ++it) {
        set_v.insert(it->nodo);
        set_v.insert(it->vizinho_char);
    }
    
    // guarda o set de vertices num vector
    // por que? porque vectors possuem indexação fixa, sets não
    for (auto i : set_v) {
        Vertice p;
        //char* char_array = new char[sizeof(i) + 1];

        strncpy(p.nodo, i.c_str(), sizeof(i));
        p.index = j;
        p.estado = 0;
        vertices_grafo.push_back(p);
        j++;
    }

    //for (int i = 0; i < vertices_grafo.size(); i++)
    //    printf("%s\n", vertices_grafo.at(i).nodo);

    j = 0;


    // constroi lista de vizinhos de cada vertice do grafo
    for (int i = 0; i < (arestas.size() ); i++) {
        
        if (retornaIndex(vertices_grafo, arestas.at(i).vizinho_char) != -1)
            aux = retornaIndex(vertices_grafo, arestas.at(i).vizinho_char);
        if (retornaIndex(vertices_grafo, arestas.at(i).nodo) != -1)
            aux2 = retornaIndex(vertices_grafo, arestas.at(i).nodo);
        //printf("vizinho: %c, aux %d j %d, i %d\n", arestas.at(i).nodo, aux, j, i);
        if (aux != -1 && aux2 != -1)
            vertices_grafo.at(aux2).lista_vizinhos.push_back(aux);
    }
    
    ordenaEnvelope(&vertices_grafo);

    if (temArcoRetorno(vertices_grafo) == false) {
        for (int i = (ordenacao.size() -1); i > 0; i--) 
            printf("%s ", ordenacao.at(i)->nodo);
        printf("%s", ordenacao.at(0)->nodo);
    } else
        printf("Erro: grafo fornecido não é um GDA.\n");

}
