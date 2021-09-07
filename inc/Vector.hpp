#ifndef VECTOR_HPP
#define VECTOR_HPP
#include <iostream>
#include <iterator>
#include <memory>

namespace ft {

template <class T, class Alloc = std::allocator<T> >
class Vector {
public:
	class Iterator;
	class Const_Iterator;
	typedef T										 value_type;
	typedef Alloc									 allocator_type;
	typedef typename allocator_type::reference		 reference;
	typedef typename allocator_type::const_reference const_reference;
	typedef typename allocator_type::pointer		 pointer;
	typedef typename allocator_type::const_pointer	 const_pointer;
	typedef Iterator								 iterator;
	typedef Const_Iterator							 const_iterator;
	typedef std::reverse_iterator<iterator>			 reverse_iterator;
	typedef std::reverse_iterator<const_iterator>	 const_reverse_iterator;
	typedef typename allocator_type::size_type		 size_type;
	typedef typename allocator_type::difference_type difference_type;

private:
	size_type	   _n;	  //size_t
	value_type	   _val;  //T
	Iterator	   _first;
	Iterator	   _last;
	allocator_type _alloc;
	const allocator_type _calloc;

public:
	//MEMBERS FUNCTIONS
	Vector(const allocator_type &alloc = allocator_type()) : _n(0), _alloc(alloc) {}																//default
	Vector(size_type n, const value_type &val = value_type(), const allocator_type &alloc = allocator_type()) : _n(n), _val(val), _calloc(alloc) {}	//fill
	// Vector(size_type n, const allocator_type &alloc = allocator_type()) : Vector<T>(n, 0), _alloc(alloc) {}											//size
	template <class InputIterator>																													//
	Vector(InputIterator first, InputIterator last, const allocator_type &alloc = allocator_type());												//range
	Vector(const Vector &x);																														//copy
	~Vector() {}
	Vector &operator=(const Vector &x);

	//ITERATORS
	iterator			   begin();
	const_iterator		   begin() const;
	iterator			   end();
	const_iterator		   end() const;
	reverse_iterator	   rbegin();
	const_reverse_iterator rbegin() const;
	reverse_iterator	   rend();
	const_reverse_iterator rend() const;

	//CAPACITY
	size_type size() const;
	size_type max_size() const;
	void	  resize(size_type sz);
	void	  resize(size_type sz, const value_type &c);
	size_type capacity() const;
	bool	  empty() const;
	void	  reserve(size_type n);

	//ELEMENTS ACCES
	reference		operator[](size_type n);
	const_reference operator[](size_type n) const;
	reference		at(size_type n);
	const_reference at(size_type n) const;

	reference		front();
	const_reference front() const;
	reference		back();
	const_reference back() const;

	//MODIFIERS
	template <class InputIterator>							   //range (1)
	void	 assign(InputIterator first, InputIterator last);  //
	void	 assign(size_type n, const value_type &val);	   //fill (2)
	void	 push_back(const value_type &x);
	void	 pop_back();
	iterator insert(iterator position, const value_type &val);					  // single element (1)
	void	 insert(iterator position, size_type n, const value_type &val);		  // fill (2)
	template <class InputIterator>												  // range (3)
	void	 insert(iterator position, InputIterator first, InputIterator last);  //
	iterator erase(iterator position);
	iterator erase(iterator first, iterator last);
	void	 swap(Vector &x);
	void	 clear();

	//ALLOCATOR
	allocator_type get_allocator() const;

	//CLASS ITERATOR
	class Iterator : public std::iterator<std::random_access_iterator_tag, T> {
	public:
		Iterator() : p(0) {}
		Iterator(T *x) : p(x) {}
		Iterator(const Iterator &mit) : p(mit.p) {}

		Iterator &operator++() {
			++p;
			return *this;
		}

		Iterator operator++(int) {
			Iterator tmp(*this);

			operator++();
			return tmp;
		}

		Iterator &operator--() {
			--p;
			return *this;
		}

		Iterator operator--(int) {
			Iterator tmp(*this);

			operator--();
			return tmp;
		}

		Iterator operator-(int x) {
			// Iterator tmp(*this);

			for ( int i = 0; i < x; i++ ) {
				// tmp.operator--();
				operator--();
			}
			return *this;
		}

		Iterator operator+(int x) {
			Iterator tmp(*this);

			for ( int i = 0; i < x; i++ )
				tmp.operator++();
			return tmp;
		}

		bool operator<(const Iterator &rhs) const { return p < rhs.p; }
		bool operator<=(const Iterator &rhs) const { return p <= rhs.p; }
		bool operator>(const Iterator &rhs) const { return p > rhs.p; }
		bool operator>=(const Iterator &rhs) const { return p >= rhs.p; }
		bool operator==(const Iterator &rhs) const { return p == rhs.p; }
		bool operator!=(const Iterator &rhs) const { return p != rhs.p; }

		T &operator*() { return *p; }

	private:
		T *p;
	};

	class ConstIterator {
		typedef typename std::random_access_iterator_tag iterator_category;
	};
};

template <class T, class Alloc>
void swap(Vector<T, Alloc> &x, Vector<T, Alloc> &y);
template <class T, class Alloc>
bool operator==(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);
template <class T, class Alloc>
bool operator!=(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);
template <class T, class Alloc>
bool operator<(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);
template <class T, class Alloc>
bool operator<=(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);
template <class T, class Alloc>
bool operator>(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);
template <class T, class Alloc>
bool operator>=(const Vector<T, Alloc> &lhs, const Vector<T, Alloc> &rhs);

}  // namespace ft
#endif
