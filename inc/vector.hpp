#ifndef VECTOR_HPP
#define VECTOR_HPP

#include <iostream>
#include <memory>

#include "iterator/random_access_iterator.hpp"
#include "iterator/reverse_iterator.hpp"
#include "utils/algorithm.hpp"

namespace ft {

template <class T, class Alloc = std::allocator<T> >
class vector {
	public:
		typedef T																						 value_type;
		typedef Alloc																				 allocator_type;
		typedef typename allocator_type::reference					 reference;
		typedef typename allocator_type::const_reference		 const_reference;
		typedef typename allocator_type::pointer						 pointer;
		typedef typename allocator_type::const_pointer			 const_pointer;
		typedef ft::random_access_iterator<value_type>			 iterator;
		typedef ft::random_access_iterator<const value_type> const_iterator;
		typedef ft::reverse_iterator<iterator>							 reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>				 const_reverse_iterator;
		typedef typename allocator_type::size_type					 size_type;
		typedef typename allocator_type::difference_type		 difference_type;

		vector( const allocator_type& alloc = allocator_type() )
				: _alloc( alloc ), _start(), _end(), _end_capacity() { }

		vector( size_type n, const value_type& val = value_type(),
						const allocator_type& alloc = allocator_type() )
				: _alloc( alloc ) {
			_start = _alloc.allocate( n );
			_end_capacity = _start + n;
			_end = _start;
			while ( n-- ) {
				_alloc.construct( _end, val );
				_end++;
			}
		}

		template <class InputIterator>
		vector( InputIterator first, InputIterator last, const allocator_type& alloc = allocator_type(),
						typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* =
								nullptr )
				: _alloc( alloc ) {
			size_type n = last - first;
			iterator	it = first;

			_start = _alloc.allocate( n );
			_end = _start;
			for ( size_type i = 0; i < n; i++ ) {
				_alloc.construct( _start + i, *( first + i ) );
				_end++;
			}
			_end_capacity = _end;
		}

		vector( const vector& x ) : _start(), _end(), _end_capacity() {
			if ( !x.empty() )
				*this = x;
		}

		~vector() { _dealloc(); }

		vector& operator=( const vector& x ) {
			_dealloc();
			_start = _alloc.allocate( x.capacity() );
			_end = _start + x.size();
			_end_capacity = _end;
			for ( size_type i = 0; i < this->size(); i++ ) {
				_alloc.construct( _start + i, x.at( i ) );
			}
			return *this;
		}

		////////////////////////////////////ITERATORS
		iterator begin() { return ( _start ); }

		const_iterator begin() const { return ( _start ); };

		iterator end() { return ( _end ); }

		const_iterator end() const { return ( _end ); }

		reverse_iterator rbegin() { return ( reverse_iterator( this->end() ) ); }

		const_reverse_iterator rbegin() const { return ( const_reverse_iterator( this->end() ) ); }

		reverse_iterator rend() { return ( reverse_iterator( this->begin() ) ); }

		const_reverse_iterator rend() const { return ( const_reverse_iterator( this->begin() ) ); }

		////////////////////////////////////CAPACITY
		size_type size() const { return ( _end - _start ); }

		size_type max_size() const {
			size_type maxVal = std::numeric_limits<size_type>::max() / sizeof( value_type );
			size_type maxAlloc = std::numeric_limits<size_type>::max() / 2;
			if ( maxVal > maxAlloc )
				return maxAlloc;
			return maxVal;
		}

		void resize( size_type sz, const value_type val = value_type() ) {
			pointer		new_start;
			size_type next_capacity;

			if ( sz > this->capacity() ) {
				( sz > this->capacity() * 2 ) ? next_capacity = sz : next_capacity = this->capacity() * 2;
				new_start = _alloc.allocate( next_capacity );
				for ( size_type i = 0; i < sz; i++ )
					_alloc.construct( new_start + i, ( i < this->size() ) ? *( _start + i ) : val );
				_dealloc();
				_start = new_start;
				_end_capacity = new_start + next_capacity;
			} else if ( sz > this->size() )
				for ( size_type i = this->size(); i < sz; i++ )
					_alloc.construct( _start + i, val );
			_end = _start + sz;
		}

		size_type capacity() const { return ( _end_capacity - _start ); }

		bool empty() const { return ( !this->size() ); }

		void reserve( size_type n ) {
			pointer		new_start;
			size_type size = this->size();

			if ( n > this->capacity() ) {
				new_start = _alloc.allocate( n );
				for ( size_type i = 0; i < size; i++ ) {
					_alloc.construct( new_start + i, *( _start + i ) );
				}
				_dealloc();
				_start = new_start;
				_end = _start + size;
				_end_capacity = _start + n;
			}
		}

		////////////////////////////////////ELEMENTS ACCES

		reference operator[]( size_type n ) { return *( _start + n ); }

		const_reference operator[]( size_type n ) const { return *( _start + n ); }

		reference at( size_type n ) {
			if ( n > size() )
				throw std::out_of_range( "vector" );
			return *( _start + n );
		}

		const_reference at( size_type n ) const {
			if ( n > size() )
				throw std::out_of_range( "vector" );
			return *( _start + n );
		}

		reference front() { return *( _start ); }

		const_reference front() const { return *( _start ); }

		reference back() { return *( _end - 1 ); }

		const_reference back() const { return *( _end - 1 ); }

		////////////////////////////////////MODIFIERS

		template <class InputIterator>
		void assign( InputIterator first, InputIterator last,
								 typename ft::enable_if<!is_integral<InputIterator>::value, InputIterator>::type* =
										 nullptr ) {
			size_type n = last - first;
			if ( n > capacity() ) {
				pointer new_start;
				new_start = _alloc.allocate( n );
				for ( size_type i = 0; i < n; i++ )
					_alloc.construct( new_start + i, first[i] );
				_dealloc();
				_set_pointers( new_start, new_start + n, new_start + n );
			} else {
				size_type i = 0;
				for ( ; i < n; i++ )
					_start[i] = first[i];
				for ( ; i < capacity(); i++ )
					_alloc.destroy( _start + i );
				_end = _start + n;
			}
		}

		void assign( size_type n, const value_type& val ) {
			pointer new_start;

			if ( n > this->capacity() ) {
				new_start = _alloc.allocate( n );
				for ( size_type i = 0; i < n; i++ ) {
					_alloc.construct( new_start + i, val );
				}
				_dealloc();
				_set_pointers( new_start, new_start + n, new_start + n );
			} else {
				size_type i = 0;
				for ( ; i < n; i++ )
					_start[i] = val;
				for ( ; i < capacity(); i++ )
					_alloc.destroy( _start + i );
				_end = _start + n;
			}
		}

		void push_back( const value_type& val ) {
			if ( !_start ) {
				_start = _alloc.allocate( 1 );
				_set_pointers( _start, _start, _start + 1 );
			} else if ( _end == _end_capacity ) {
				size_type next_capacity = capacity() * 2;
				pointer		new_start = _alloc.allocate( next_capacity );
				size_type i = 0;
				while ( i < size() ) {
					_alloc.construct( new_start + i, _start[i] );
					i++;
				}
				_dealloc();
				_set_pointers( new_start, new_start + i, new_start + next_capacity );
			}
			_alloc.construct( _end, val );
			_end++;
		}

		void pop_back() { _alloc.destroy( --_end ); }

		iterator insert( iterator position, const value_type& val ) {
			return insert( position, 1, val );
		}

		iterator insert( iterator position, size_type n, const value_type& val ) {
			pointer		new_start;
			pointer		new_end;
			size_type new_capacity = capacity();
			size_type pos_len = position.base() - _start;
			size_type i = 0;

			if ( ( size() + n ) >= new_capacity )
				new_capacity *= 2;
			new_start = _alloc.allocate( new_capacity );
			new_end = new_start;
			for ( ; i < pos_len; i++ )
				_alloc.construct( new_end++, _start[i] );
			for ( ; n > 0; n-- )
				_alloc.construct( new_end++, val );
			for ( ; i < size(); i++ )
				_alloc.construct( new_end++, _start[i] );
			_dealloc();
			_set_pointers( new_start, new_end, new_start + new_capacity );
			return _start + pos_len;
		}

	public:
		template <class InputIterator>
		void
		insert( iterator position, InputIterator first, InputIterator last,
						typename ft::enable_if<!ft::is_integral<InputIterator>::value, InputIterator>::type* =
								nullptr ) {
			pointer		new_start;
			pointer		new_end;
			size_type new_capacity = capacity();
			size_type pos_len = position.base() - _start;
			size_type i = 0;

			if ( ( size() + ( last - first ) ) >= new_capacity )
				new_capacity *= 2;
			new_start = _alloc.allocate( new_capacity );
			new_end = new_start;
			for ( ; i < pos_len; i++ )
				_alloc.construct( new_end++, _start[i] );
			for ( ; first != last; first++ )
				_alloc.construct( new_end++, *first );
			for ( ; i < size(); i++ )
				_alloc.construct( new_end++, *( _start + i ) );
			_dealloc();
			_set_pointers( new_start, new_end, new_start + new_capacity );
		}

		iterator erase( iterator position ) {
			pointer p = _start + ( position - begin() );
			_move( p + 1, _end, p );
			_alloc.destroy( --_end );
			return p;
		}

		iterator erase( iterator first, iterator last ) {
			pointer p = _start + ( first - begin() );
			if ( first != last ) {
				p = _move( p + ( last - first ), _end, p );
				while ( _end > p )
					_alloc.destroy( --_end );
			}
			return p;
		}

	private:
		template <class _Tp, class _Up>
		pointer _move( _Tp* __first, _Tp* __last, _Up* __result ) {
			const size_t __n = static_cast<size_t>( __last - __first );
			if ( __n > 0 )
				std::memmove( __result, __first, __n * sizeof( _Up ) );
			return __result + __n;
		}

	public:
		void swap( vector& x ) {
			std::swap( _start, x._start );
			std::swap( _end, x._end );
			std::swap( _end_capacity, x._end_capacity );
		}

		void clear() {
			size_type size = this->size();
			for ( size_type i = 0; i < size; i++ ) {
				_end--;
				_alloc.destroy( _end );
			}
		}

		////////////////////////////////////ALLOCATOR
		allocator_type get_allocator() const { return _alloc; };

	private:
		void _set_pointers( pointer s, pointer e, pointer e_c ) {
			_start = s;
			_end = e;
			_end_capacity = e_c;
		}

	private:
		void _dealloc( void ) {
			this->clear();
			_alloc.deallocate( _start, this->capacity() );
		}

	private:
		allocator_type _alloc;
		pointer				 _start;
		pointer				 _end;
		pointer				 _end_capacity;
};

// https://www.cplusplus.com/reference/vector/vector/operators/

template <class T, class Alloc>
void swap( vector<T, Alloc>& x, vector<T, Alloc>& y ) {
	x.swap( y );
}

template <class T, class Alloc>
bool operator==( const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs ) {
	typename vector<T>::const_iterator lhsIt = lhs.begin();
	typename vector<T>::const_iterator rhsIt = rhs.begin();

	while ( lhsIt != lhs.end() )
		if ( lhs.size() != rhs.size() || *lhsIt++ != *rhsIt++ )
			return false;
	return true;
}

template <class T, class Alloc>
bool operator!=( const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs ) {
	return ( !( lhs == rhs ) );
}

template <class T, class Alloc>
bool operator<( const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs ) {
	typename vector<T>::const_iterator lhsItb = lhs.begin();
	typename vector<T>::const_iterator lhsIte = lhs.end();
	typename vector<T>::const_iterator rhsItb = rhs.begin();
	typename vector<T>::const_iterator rhsIte = rhs.end();

	return ft::lexicographical_compare( lhsItb, lhsIte, rhsItb, rhsIte );
}

template <class T, class Alloc>
bool operator<=( const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs ) {
	return ( !( rhs < lhs ) );
}

template <class T, class Alloc>
bool operator>( const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs ) {
	return ( rhs < lhs );
}

template <class T, class Alloc>
bool operator>=( const vector<T, Alloc>& lhs, const vector<T, Alloc>& rhs ) {
	return ( !( lhs < rhs ) );
}

} // namespace ft
#endif
