#include <iostream>


template <class T> class LinkedList{
 private:
  size_t next[20];
  size_t previous[20];
  T data[20];
  size_t n_elements;
 public:
  LinkedList(T head_value, T tail_value)
  {
    data[0] = head_value;
    data[1] = tail_value;

    next[0] = 1;
    previous[1] = 0;

    n_elements = 0;
  }

  void pushHead (T value)
  {
    data[n_elements + 2] = value;

    next[n_elements + 2] = next[0];
    previous[next[0]] = n_elements + 2;

    next[0] = n_elements + 2;
    previous[n_elements + 2] = 0;

    n_elements++;
  }

  void pushTail (T value)
  {
    data[n_elements + 2] = value;

    previous[n_elements + 2] = previous[1];
    next[previous[1]] = n_elements + 2;

    previous[1] = n_elements + 2;
    next[n_elements + 2] = 1;

    n_elements++;
  }

  void dump (const char *filename)
  {
    FILE *file = fopen (filename, "w");

    fprintf (file, "digraph {\n");


    size_t current_node = next[0];
    fprintf (file, "node%d[label=\"{{%d}|{VALUE | %d}}\",shape=record];\n", current_node, current_node, data[current_node]);
    current_node = next[current_node];
    while (data[current_node] != data[1])
      {
        fprintf (file, "node%d[label=\"{{%d}|{VALUE | %d}}\",shape=record];\n", current_node, current_node, data[current_node]);
        fprintf (file, "node%d -> node%d;\n", current_node, previous[current_node]);
        fprintf (file, "node%d -> node%d;\n", previous[current_node], current_node);
        current_node = next[current_node];
      }
    fprintf (file, "head -> node%d;\n", next[0]);
    fprintf (file, "node%d -> tail;\n", previous[1]);
    fprintf (file, "}");
    fclose (file);
    system ("dot -Tpng dump.dot > dump.png");
  }


};

int main ()
{
  LinkedList<int> my_list = LinkedList (0, 65536);
  my_list.pushTail (10);
  my_list.pushTail (20);
  my_list.pushTail (30);
  my_list.pushTail (40);
  my_list.pushHead (50);

  my_list.dump("dump.dot");

  return 0;
}