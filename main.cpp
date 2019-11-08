#include <iostream>
#include <stack>
#include <cstring>

template<class T>
class LinkedList {
 private:
  size_t *next;
  size_t *previous;
  T *data;
  std::stack<int> empty_cells;
  size_t n_elements;
  size_t max_elements;
  size_t head;
  size_t tail;

  void interlinkNodes(size_t prev_node, size_t next_node)
  {
    next[prev_node] = next_node;
    previous[next_node] = prev_node;
  }

 public:
  LinkedList (size_t max_elements) : n_elements (0)
  {
    this->max_elements = max_elements;

    for (int i = max_elements - 1; i >= 0; i--) empty_cells.push (i);

    data = new T[max_elements] ();
    next = new size_t[max_elements] ();
    previous = new size_t[max_elements] ();
  }

  ~LinkedList()
  {
    delete next;
    delete previous;
    delete data;
  }

  bool check ()
  {
    size_t cnt = 0;
    size_t current = head;
    while (cnt < n_elements && current != tail)
      {
        current = next[current];
        cnt++;
      }
    if (cnt != n_elements || current != tail)
      {
        perror ("List is broken! Tail is unreachable or number of elements is wrong!");
        return false;
      }

    cnt = 0;
    current = head;
    while (cnt < n_elements && current != head)
      {
        current = previous[current];
        cnt++;
      }

    if (cnt != n_elements || current != head)
      {
        perror ("List is broken! Head is unreachable or number of elements is wrong!");
        return false;
      }
    return true;
  }

  size_t getHead ()
  {
    return data[head];
  }

  size_t getTail ()
  {
    return data[tail];
  }

  size_t getElementAfter (const size_t position)
  {
    return next[position];
  }

  size_t getElementBefore (const size_t position)
  {
    return previous[position];
  }

  void clear ()
  {
    head = 0;
    tail = 0;
    n_elements = 0;

    memset (next, 0, max_elements);
    memset (previous, 0, max_elements);
    memset (data, 0, max_elements);

    for (int i = max_elements - 1; i >= 0; i--) empty_cells.push (i);
  }

  void deleteElement (const size_t position)
  {
    if (position > max_elements)
      {
        perror ("Trying to delete element with position greater than max number of elements in list!");
        return;
      }
    empty_cells.push (position);

    data[position] = 0;
    interlinkNodes (previous[position], next[position]);

    n_elements--;
  }

  void pushHead (const T value)
  {
    int cell = empty_cells.top ();
    empty_cells.pop ();
    data[cell] = value;
    if (n_elements)
      {
        interlinkNodes (cell, head);
      }
    else
      {
        tail = cell;
      }
    head = cell;

    n_elements++;
  }

  void pushTail (const T value)
  {
    int cell = empty_cells.top ();
    empty_cells.pop ();
    data[cell] = value;

    if (n_elements)
      {
        interlinkNodes (tail, cell);
      }
    else
      {
        head = cell;
      }
    tail = cell;

    n_elements++;
  }

  bool insertAfter (const size_t position, const T value)
  {
    if (position == tail)
      {
        pushTail (value);
        return true;
      }

    int cell = empty_cells.top ();
    empty_cells.pop ();
    data[cell] = value;

    interlinkNodes (cell, next[position]);
    interlinkNodes (position, cell);

    n_elements++;
    return true;
  }

  bool insertBefore (const size_t position, const T value)
  {
    if (position == head)
      {
        pushHead (value);
        return true;
      }

    int cell = empty_cells.top ();
    empty_cells.pop ();
    data[cell] = value;

    interlinkNodes (previous[position], cell);
    interlinkNodes (cell, position);

    n_elements++;
    return true;
  }

  size_t getElementByListPosition (size_t position)
  {
    if (position > n_elements)
      {
        perror ("Trying to access element with position greater than number of elements in list!");
        return 0;
      }
    if (position > n_elements / 2)
      {
        position = n_elements - position;
        size_t curr = tail;
        while (position)
          {
            curr = previous[curr];
            position--;
          }
        return data[curr];
      }

    size_t curr = head;
    while (position)
      {
        curr = next[curr];
        position--;
      }
     return data[curr];
  }

  void dump (const char *filename)
  {
    FILE *file = fopen (filename, "w");

    fprintf (file, "digraph {\n");

    size_t current_node = head;
    fprintf (file, "node%d[label=\"{{%d}|{{VALUE|%d}|{NEXT|%d}|{PREVIOUS|%d}}}\",shape=record];\n", current_node, current_node, data[current_node], next[current_node], previous[current_node]);
    current_node = next[current_node];
    bool end = false;
    while (!end)
      {
        if (current_node == tail) end = true;
        fprintf (file, "node%d[label=\"{{%d}|{{VALUE|%d}|{NEXT|%d}|{PREVIOUS|%d}}}\",shape=record];\n", current_node, current_node, data[current_node], next[current_node], previous[current_node]);
        fprintf (file, "node%d -> node%d;\n", current_node, previous[current_node]);
        fprintf (file, "node%d -> node%d;\n", previous[current_node], current_node);
        current_node = next[current_node];
      }
    fprintf (file, "head -> node%d;\n", head);
    fprintf (file, "tail -> node%d;\n", tail);
    fprintf (file, "}");
    fclose (file);
  }

};

bool testPushHead ();
bool testPushTail ();
bool testGetHead ();
bool testGetTail ();
bool testInsertAfter ();
bool testInsertBefore ();
bool testDeleteElement ();

int main ()
{
  testPushHead ();
  testPushTail ();
  testGetHead ();
  testGetTail ();
  testInsertAfter ();
  testInsertBefore ();
  testDeleteElement ();

  LinkedList<int> list = LinkedList<int>(4);

  list.pushHead(10);
  list.pushHead (20);
  list.pushHead (30);
  list.dump("dump.dot");

  return 0;
}

bool testPushHead ()
{
  LinkedList<int> test_list = LinkedList<int>(10);
  test_list.pushHead (10);
  int element = test_list.getElementByListPosition (0);
  if (element == 10)
    {
      std::cout << "PushHead test passed!" << std::endl;
      return true;
    }
  std::cout << "PushHead test failed! Expected 10 got " << element << "!" << std::endl;
  return false;
}

bool testPushTail ()
{
  LinkedList<int> test_list = LinkedList<int>(10);
  test_list.pushTail (10);
  int element = test_list.getElementByListPosition (0);
  if (element == 10)
    {
      std::cout << "PushTail test passed!" << std::endl;
      return true;
    }
  std::cout << "PushTail test failed! Expected 10 got " << element << "!" << std::endl;
  return false;
}

bool testGetHead ()
{
  LinkedList<int> test_list = LinkedList<int>(10);
  test_list.pushHead (10);
  test_list.pushHead (20);
  int head = test_list.getHead ();

  if (head == 20)
    {
      std::cout << "GetHead test passed!" << std::endl;
      return true;
    }
  std::cout << "GetHead test failed! Expected 20 got " << head << "!" << std::endl;
  return false;
}

bool testGetTail ()
{
  LinkedList<int> test_list = LinkedList<int>(10);
  test_list.pushHead (10);
  test_list.pushHead (20);
  int head = test_list.getTail ();

  if (head == 10)
    {
      std::cout << "GetTail test passed!" << std::endl;
      return true;
    }
  std::cout << "GetTail test failed! Expected 10 got " << head << "!" << std::endl;
  return false;
}

bool testInsertAfter ()
{
  LinkedList<int> test_list = LinkedList<int>(10);
  test_list.pushHead (10);
  test_list.pushHead (20);
  test_list.pushHead (30);

  size_t node = test_list.getElementByListPosition (1);

  test_list.insertAfter (node, 50);

  node = test_list.getElementByListPosition (3);

  if (node == 50)
    {
      std::cout << "InsertAfter test passed!" << std::endl;
      return true;
    }
  std::cout << "InsertAfter test failed! Expected 50 got " << node << "!" << std::endl;
  return false;
}

bool testInsertBefore ()
{
  LinkedList<int> test_list = LinkedList<int>(10);
  test_list.pushHead (10);
  test_list.pushHead (20);
  test_list.pushHead (30);

  size_t node = test_list.getElementByListPosition (1);

  test_list.insertBefore (node, 20);

  node = test_list.getElementByListPosition (3);

  if (node == 20)
    {
      std::cout << "InsertBefore test passed!" << std::endl;
      return true;
    }
  std::cout << "InsertBefore test failed! Expected 50 got " << node << "!" << std::endl;
  return false;
}

bool testDeleteElement ()
{
  LinkedList<int> test_list = LinkedList<int>(10);
  test_list.pushHead (10);
  test_list.pushHead (20);
  test_list.pushHead (30);

  test_list.deleteElement (1);

  size_t node = test_list.getElementByListPosition (1);

  if (node == 10)
    {
      std::cout << "DeleteElement test passed!" << std::endl;
      return true;
    }
  std::cout << "DeleteElement test failed! Expected 10 got " << node << "!" << std::endl;
  return false;
}