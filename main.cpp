#include <iostream>
#include <stack>


template<class T>
class LinkedList {
 private:
  size_t* next;
  size_t* previous;
  T* data;
  std::stack<int> empty_cells;
  size_t n_elements;
  size_t max_elements;
  size_t head;
  size_t tail;
 public:
  LinkedList (size_t max_elements) : n_elements(0)
  {
    this->max_elements = max_elements;

    for(int i = max_elements - 1; i >=0; i--) empty_cells.push(i);

    data = new T[max_elements]();
    next = new size_t[max_elements]();
    previous = new size_t[max_elements]();
  }

  void deleteElement(size_t position)
  {
    if (position > max_elements)
      {
        perror ("Trying to delete element with position greater than max number of elements in list!");
        return;
      }
    empty_cells.push(position);

    data[position] = 0;
    previous[next[position]] = previous[position];
    next[previous[position]] = next[position];

    n_elements--;
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
  LinkedList<int> my_list = (10);
  my_list.pushTail (10);
  my_list.pushTail (30);
  my_list.pushTail (50);
  my_list.deleteElement (1);
  my_list.dump("dump.dot");

  return 0;
}