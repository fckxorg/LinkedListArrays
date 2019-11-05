#include <iostream>
#include <stack>

const int LIST_MAX_SIZE = 20;

template<class T>
class LinkedList {
 private:
  size_t next[LIST_MAX_SIZE] = {};
  size_t previous[LIST_MAX_SIZE] = {};
  T data[LIST_MAX_SIZE] = {};
  std::stack<int> empty_cells;
  size_t n_elements;
  size_t head;
  size_t tail;
 public:
  LinkedList () : n_elements(0)
  {
    for(int i = LIST_MAX_SIZE - 1; i >=0; i--) empty_cells.push(i);
  }

  void pushHead (T value)
  {
    int cell = empty_cells.top(); empty_cells.pop();
    data[cell] = value;
    if (n_elements)
      {
        next[cell] = head;
        previous[head] = cell;
      }
    else
      {
        tail = cell;
      }
    head = cell;

    n_elements++;
  }

  void pushTail (T value)
  {
    int cell = empty_cells.top(); empty_cells.pop();
    data[cell] = value;

    if (n_elements)
      {
        previous[cell] = tail;
        next[tail] = cell;
      }
    else
      {
        head = cell;
      }
    tail = cell;

    n_elements++;
  }

  bool insertAfter (size_t position, T value)
  {
    if(position == tail)
      {
        pushTail (value);
        return true;
      }

    int cell = empty_cells.top(); empty_cells.pop();
    data[cell] = value;

    previous[next[position]] = cell;
    next[cell] = next[position];

    next[position] = cell;
    previous[cell] = position;

    n_elements++;
    return true;
  }

  bool insertBefore (size_t position, T value)
  {
    if(position == head)
      {
        pushHead (value);
        return true;
      }

    int cell = empty_cells.top(); empty_cells.pop();
    data[cell] = value;

    next[previous[position]] = cell;
    previous[cell] = previous[position];

    previous[position] = cell;
    next[cell] = position;

    n_elements++;
    return true;
  }

  void dump (const char *filename)
  {
    FILE *file = fopen (filename, "w");

    fprintf (file, "digraph {\n");

    size_t current_node = head;
    fprintf (file, "node%d[label=\"{{%d}|{VALUE | %d}}\",shape=record];\n", current_node, current_node, data[current_node]);
    current_node = next[current_node];
    bool end = false;
    while (!end)
      {
        if(current_node == tail) end = true;
        fprintf (file, "node%d[label=\"{{%d}|{VALUE | %d}}\",shape=record];\n", current_node, current_node, data[current_node]);
        fprintf (file, "node%d -> node%d;\n", current_node, previous[current_node]);
        fprintf (file, "node%d -> node%d;\n", previous[current_node], current_node);
        current_node = next[current_node];
      }
    fprintf (file, "head -> node%d;\n", head);
    fprintf (file, "tail -> node%d;\n", tail);
    fprintf (file, "}");
    fclose (file);
    system ("dot -Tpng dump.dot > dump.png");
  }

};

int main ()
{
  LinkedList<int> my_list;
  my_list.pushTail (10);
  my_list.pushTail (20);
  my_list.pushTail (30);
  my_list.pushTail (40);
  my_list.pushHead (50);
  my_list.insertAfter (3, 100);
  my_list.insertBefore (3, 200);

  my_list.dump ("dump.dot");

  return 0;
}