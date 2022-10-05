#ifndef LIST_H_
#define LIST_H_




namespace dus
{
struct node_base
{
    node_base* next_;
    node_base* prev_;

    node_base() : next_(nullptr), prev_(nullptr) {}

    void hook(node_base* const pos)
    {
        next_ = pos;
        prev_ = pos->prev_;
        pos->prev_->next_ = this;
        pos->prev_ = this;
    }

    void unhook()
    {
      node_base* const next = next;
      node_base* const prev = prev;
      prev->next_ = next;
      next->prev_ = prev;
    }

    void print()
    {
        std::cout << this << ":" << next_ << " : " << prev_ << std::endl;
        std::cout << this << ":" << prev_->next_ << ": :" << prev_->prev_ << std::endl;
        std::cout << this << ":" << next_->next_ << ":::" << next_->prev_ << std::endl;
    }
};


struct node_header : public node_base
{
    size_t size_;

    node_header()
    {
        next_ = prev_ = this;
        std::cout << this << ":" << next_ << " | " << prev_ << std::endl;
        size_ = 0;
    }
};


template <typename T>
struct node_data : public node_base
{
    T data_;
    node_data(const T& value) : data_(value) {}
};


template<typename T>
struct iterator_base
{
    node_base* node_;
    iterator_base() {}
    iterator_base(node_base* node) : node_(node) {}

    bool operator == (iterator_base const& rhs) const { return node_ == rhs.node_; }
    bool operator != (iterator_base const& rhs) const { return !(*this == rhs); }

    iterator_base& operator ++ () { node_ = node_->next_; return *this;}
    iterator_base& operator -- () { node_ = node_->prev_; return *this;}
    iterator_base operator ++ (T)
    {
        iterator_base t = *this;
        node_ = node_->next_;
        return t;
    }

    iterator_base operator -- (T)
    {
        iterator_base t = *this;
        t.node_ = node_->prev_;
        return t;
    }

    T& operator*()  const { return static_cast<node_data<T>*>(node_)->data_; }
    T* operator->() const { return &static_cast<node_data<T>*>(node_)->data_; }
};


template <typename T>
class list
{
public:
    using iterator       = iterator_base<T>;
    using const_iterator = iterator_base<T const>;

    list() {}
    ~list() {}

    list(const list<T>& other) {}
    list(list<T>&& other) {}

    list& operator = (const list<T>& rhs);
    list& operator = (list<T>&& rhs);

    size_t size() const { return node_header_.size_; }

    iterator begin() { return iterator(node_header_.next_); }
    iterator end()   { return iterator(&node_header_); }
    const_iterator begin() const { return const_iterator(&node_header_.next_); }
    const_iterator end()   const { return const_iterator(&node_header_); }

    void push_back(const T& value) { insert(end(), value); }

    void print()
    {
        node_header_.print();

        for(iterator it = begin(); it != end(); it++)
        {
            // std::cout << *it << " ";
            it.node_->print();
        }
        std::cout << "-----------------" << std::endl;
    }

private:
    node_header node_header_;

    void insert(iterator pos, const T& value)
    {
        node_data<T>* node = new node_data<T>(value);
        node->hook(pos.node_);
        node_header_.size_++;

    }

};

}














// namespace dus
// {

// template <typename T>
// class list
// {

//     struct node
//     {
//         T     data_;
//         node* next_;
//         node* prev_;

//         node(const T& data) : data_(data), next_(nullptr), prev_(nullptr) {}
//         node(T&& data) : data_(std::forward<T>(data)), next_(nullptr), prev_(nullptr) { }

//         node& operator = (const node& rhs) = delete;
//         node& operator = (node&& rhs) = delete;

//         ~node() {}
//     };

//     template<typename R>
//     struct iterator_base
//     {
//         node* current_;
//         iterator_base() : current_(nullptr) {}
//         iterator_base(node* current) : current_(current) {}

//         bool operator == (iterator_base const& rhs) const { return current_ == rhs.current_; }
//         bool operator != (iterator_base const& rhs) const { return !(*this == rhs); }

//         iterator_base& operator ++ () { current_ = current_->next_; return *this;}
//         iterator_base& operator -- () { current_ = current_->prev_; return *this;}
//         iterator_base operator ++ (R) 
//         {
//             iterator_base t = *this;
//             current_ = current_->next_;
//             return t; 
//         }
        
//         iterator_base operator -- (R) 
//         {
//             iterator_base t = *this;
//             t.current_ = current_->prev_;
//             return t; 
//         }

//         R& operator*()  const { return current_->data_; }
//         R* operator->() const { return &current_->data_; }
//     };

// public:
//     using iterator       = iterator_base<T>;
//     using const_iterator = iterator_base<T const>;

//     list() : head_(nullptr), tail_(nullptr) {}
//     ~list() {}

//     list(const list<T>& other) {}
//     list(list<T>&& other) {}

//     list& operator = (const list<T>& rhs);
//     list& operator = (list<T>&& rhs);


//     size_t size() const
//     {
//         node* p     = head_;
//         size_t size = 0;

//         while(p) 
//         {
//             p = p->next_;
//             size++;
//         }
//         return size;
//     }


//     void push(const T& value) { push(new node(value)); }
//     void push(T&& value) { push(new node(std::forward<T>(value))); }

//     void pop()
//     {
//         if (!head_) { return; }

//         const node* last = tail_;
//         if (head_ != tail_)
//         {
//             tail_ = tail_->prev_;
//             tail_->next_ = nullptr;
//         }
//         else
//         {
//             head_ = nullptr;
//             tail_ = nullptr;
//         }
//         delete last;
//     }

//     iterator insert(iterator pos, const T& value)
//     {
//         // pos->current_ = new node(value);

//     }

//     void remove(const T& value) {}
//     void reverse() {}
//     void sort() {}




//     iterator begin() { return iterator(head_); }
//     iterator end()   { return tail_ ? iterator(tail_->next_) : iterator(); }
//     const_iterator begin() const { return const_iterator(head_); }
//     const_iterator end()   const { return tail_ ? iterator(tail_->next_) : iterator(); }


// private:
//     /* first node in the list */
//     node* head_;
//     /* last node in the list */
//     node* tail_;

//     void push(node* p)
//     {
//         /* list is empty */
//         if (!tail_)
//         {
//             tail_ = head_ = p;
//             return;
//         }

//         p->prev_ = tail_;
//         tail_->next_ = p;
//         tail_ = p;

//     }
// };

// }












/************************************************************************************************/


namespace ds
{

template<typename T>
class LinkedList
{
    struct Node
    {
        Node(Node* prev, Node* next)
            : prev(prev)
            , next(next)
        {
            prev->next = this;
            next->prev = this;
        }
        virtual ~Node() {}

        Node*       next;
        Node*       prev;
    };
    struct DataNode: public Node
    {
        DataNode(Node* prev, Node* next, T const& data)
            : Node(prev, next)
            , data(data)
        {}
        DataNode(Node* prev, Node* next, T&& data)
            : Node(prev, next)
            , data(std::forward<T>(data))
        {}
        template<typename... Args>
        DataNode(Node* prev, Node* next, Args&& ...args)
            : Node(prev, next)
            , data(std::forward<Args>(args)...)
        {}
        T           data;
    };

    Node    sentinel;

    template<typename ResultType>
    struct IteratorBase
    {
        Node* current;
        IteratorBase(Node* node)
            : current(node)
        {}
        bool operator==(IteratorBase const& rhs) const      {return current == rhs.current;}
        bool operator!=(IteratorBase const& rhs) const      {return !(*this == rhs);}

        IteratorBase& operator++()                          {current = current->next; return *this;}
        IteratorBase& operator--()                          {current = current->prev; return *this;}
        IteratorBase  operator++(int)                       {IteratorBase result(*this); ++(*this); return result;}
        IteratorBase  operator--(int)                       {IteratorBase result(*this); --(*this); return result;}

        ResultType& operator*() const                       {return static_cast<DataNode*>(current)->data;}
        ResultType* operator->() const                      {return &static_cast<DataNode*>(current)->data;}
    };
public:
    using iterator          = IteratorBase<T>;
    using const_iterator    = IteratorBase<T const>;

    // Default
    LinkedList()
        : sentinel(&sentinel, &sentinel)
    {}
    // Copying␣
    LinkedList(LinkedList const& copy)
        : LinkedList()
    {
        for (auto const& val : copy) {
            push_back(val);
        }
    }
    LinkedList& operator=(LinkedList const& rhs)
    {
        LinkedList      tmp(rhs);
        tmp.swap(*this);
        return *this;
    }
    // Moving
    LinkedList(LinkedList&& move) noexcept
        : LinkedList()
    {
        move.swap(*this);
    }
    LinkedList& operator=(LinkedList&& rhs) noexcept
    {
        rhs.swap(*this);
        return *this;
    }
    // Destroying
    ~LinkedList()
    {
        Node* next;
        for (Node* loop = sentinel.next; loop != &sentinel; loop = next)
        {
            next = loop->next;
            delete loop;
        }
    }
    // Swap
    void swap(LinkedList& other) noexcept
    {
        using std::swap;
        swap(sentinel.next->prev, other.sentinel.next->prev);
        swap(sentinel.prev->next, other.sentinel.prev->next);
        swap(sentinel.next, other.sentinel.next);
        swap(sentinel.prev, other.sentinel.prev);
    }

    // Copying value into list
    void push_back(T const& value)      {new DataNode(sentinel.prev, &sentinel, value);}
    void push_front(T const& value)     {new DataNode(&sentinel, sentinel.next, value);}

    // Moving value into list
    void push_back(T&& value)           {new DataNode(sentinel.prev, &sentinel, std::forward<T>(value));}
    void push_front(T&& value)          {new DataNode(&sentinel, sentinel.next, std::forward<T>(value));}

    // Emplaning value into list
    template<typename... Args>
    void emplace_back(Args&& ...args)   {new DataNode(sentinel.prev, &sentinel, std::forward<Args>(args)...);}
    template<typename... Args>
    void emplace_front(Args&& ...args)  {new DataNode(&sentinel, sentinel.next, std::forward<Args>(args)...);}


    iterator begin()                    {return iterator(sentinel.next);}
    iterator end()                      {return iterator(&sentinel);}
    const_iterator begin()  const       {return const_iterator(sentinel.next);}
    const_iterator end()    const       {return const_iterator(&sentinel);}
    const_iterator cbegin() const       {return const_iterator(sentinel.next);}
    const_iterator cend()   const       {return const_iterator(&sentinel);}
};

template<typename T>
void swap(LinkedList<T>& lhs, LinkedList<T>& rhs)
{
    lhs.swap(rhs);
}
}

#endif