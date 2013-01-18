#ifndef _OLA_ASE_MATH_H__
#define _OLA_ASE_MATH_H__


#define     QUATERNION_NORMALIZATION_THRESHOLD  64
#define     PI              float(3.1415926535897932384626433832795)
#define     EPSILON         float(10e-6)
#define     RAD_TO_DEG      float(57.2957795130823208767981548141052)
#define     DEG_TO_RAD      float(0.0174532925199432957692369076848861)
#define     equivalent(a,b)     (((a < b + EPSILON) && (a > b - EPSILON)) ? true : false)

namespace asemath
{
	inline float to_degrees(float radians) { return radians*RAD_TO_DEG; }
	inline float to_radians(float degrees) { return degrees*DEG_TO_RAD; }

	typedef float real;

	template <int N, class T>	
	class vec
	{				
	public:
		int size() const { return N; }

		vec(const T & t = T()) 
		{ for(int i = 0; i < N; i++) v[i] = t; }
		vec(const T * tp)
		{ for(int i = 0; i < N; i++) v[i] = tp[i]; }

		const T * get_value() const
		{ return v; }


		T dot( const vec<N,T> & rhs ) const
		{ 
			T r = 0;
			for(int i = 0; i < N; i++) r += v[i]*rhs.v[i];
			return r;
		}

		T length() const
		{
			T r = 0;
			for(int i = 0; i < N; i++) r += v[i]*v[i]; 
			return T(sqrt(r));
		}	

		T square_norm() const
		{
			T r = 0;
			for(int i = 0; i < N; i++) r += v[i]*v[i]; 
			return r;
		}	

		void  negate()
		{ for(int i = 0; i < N; i++) v[i] = -v[i]; }


		T normalize() 
		{ 
			T sum(0);
			for(int i = 0; i < N; i++) 
				sum += v[i]*v[i];
			sum = T(sqrt(sum));
			if (sum > EPSILON)
				for(int i = 0; i < N; i++) 
					v[i] /= sum;
			return sum;
		}


		vec<N,T> & set_value( const T * rhs )
		{ for(int i = 0; i < N; i++) v[i] = rhs[i]; return *this; }

		T & operator [] ( int i )
		{ return v[i]; }

		const T & operator [] ( int i ) const
		{ return v[i]; }

		vec<N,T> & operator *= ( T d )
		{ for(int i = 0; i < N; i++) v[i] *= d; return *this;}

		vec<N,T> & operator *= ( const vec<N,T> & u )
		{ for(int i = 0; i < N; i++) v[i] *= u[i]; return *this;}

		vec<N,T> & operator /= ( T d )
		{ if(d == 0) return *this; for(int i = 0; i < N; i++) v[i] /= d; return *this;}

		vec<N,T> & operator += ( const vec<N,T> & u )
		{ for(int i = 0; i < N; i++) v[i] += u.v[i]; return *this;}

		vec<N,T> & operator -= ( const vec<N,T> & u )
		{ for(int i = 0; i < N; i++) v[i] -= u.v[i]; return *this;}


		vec<N,T> operator - () const
		{ vec<N,T> rv = v; rv.negate(); return rv; }

		vec<N,T> operator + ( const vec<N,T> &v) const
		{ vec<N,T> rt(*this); return rt += v; }

		vec<N,T> operator - ( const vec<N,T> &v) const
		{ vec<N,T> rt(*this); return rt -= v; }

		vec<N,T> operator * ( T d) const
		{ vec<N,T> rt(*this); return rt *= d; }

		//protected:
		T v[N];
	};

	class vec3 : public vec<3,real>
	{
	public:
		vec3(const real & t = real()) : vec<3,real>(t)
		{}
		vec3(const vec<3,real> & t) : vec<3,real>(t)
		{}
		vec3(const real * tp) : vec<3,real>(tp)
		{}

		vec3(real x, real y, real z)
		{ v[0] = x; v[1] = y; v[2] = z; }

		void get_value(real & x, real & y, real & z) const
		{ x = v[0]; y = v[1]; z = v[2]; }

		vec3 cross( const vec3 &rhs ) const
		{
			vec3 rt;
			rt.v[0] = v[1]*rhs.v[2]-v[2]*rhs.v[1];
			rt.v[1] = v[2]*rhs.v[0]-v[0]*rhs.v[2];
			rt.v[2] = v[0]*rhs.v[1]-v[1]*rhs.v[0];	
			return rt;
		}

		vec3 & set_value( const real & x, const real & y, const real & z)
		{ v[0] = x; v[1] = y; v[2] = z; return *this; }

	};

	class vec4 : public vec<4,real>
	{
	public:
		vec4(const real & t = real()) : vec<4,real>(t)
		{}
		vec4(const vec<4,real> & t) : vec<4,real>(t)
		{}

		vec4(const vec<3,real> & t, real fourth)

		{ v[0] = t.v[0]; v[1] = t.v[1]; v[2] = t.v[2]; v[3] = fourth; }
		vec4(const real * tp) : vec<4,real>(tp)
		{}
		vec4(real x, real y, real z, real w)
		{ v[0] = x; v[1] = y; v[2] = z; v[3] = w; }

		void get_value(real & x, real & y, real & z, real & w) const
		{ x = v[0]; y = v[1]; z = v[2]; w = v[3]; }

		vec4 & set_value( const real & x, const real & y, const real & z, const real & w)
		{ v[0] = x; v[1] = y; v[2] = z; v[3] = w; return *this; }
	};

	class matrix4
	{

	public:

		matrix4() { make_identity(); }

		matrix4( real r ) 
		{ set_value(r); }

		matrix4( real * m )
		{ set_value(m); }

		matrix4( real a00, real a01, real a02, real a03,
			real a10, real a11, real a12, real a13,
			real a20, real a21, real a22, real a23,
			real a30, real a31, real a32, real a33 )
		{
			element(0,0) = a00;
			element(0,1) = a01;
			element(0,2) = a02;
			element(0,3) = a03;

			element(1,0) = a10;
			element(1,1) = a11;
			element(1,2) = a12;
			element(1,3) = a13;

			element(2,0) = a20;
			element(2,1) = a21;
			element(2,2) = a22;
			element(2,3) = a23;

			element(3,0) = a30;
			element(3,1) = a31;
			element(3,2) = a32;
			element(3,3) = a33;
		}


		void get_value( real * mp ) const
		{
			int c = 0;
			for(int j=0; j < 4; j++)
				for(int i=0; i < 4; i++)
					mp[c++] = element(i,j);
		}


		const real * get_value() const
		{ return m; }

		void set_value( real * mp)
		{
			int c = 0;
			for(int j=0; j < 4; j++)
				for(int i=0; i < 4; i++)
					element(i,j) = mp[c++];
		}

		void set_value( real r ) 
		{
			for(int i=0; i < 4; i++)
				for(int j=0; j < 4; j++)
					element(i,j) = r;
		}

		void make_identity()
		{
			element(0,0) = 1.0;
			element(0,1) = 0.0;
			element(0,2) = 0.0; 
			element(0,3) = 0.0;

			element(1,0) = 0.0;
			element(1,1) = 1.0; 
			element(1,2) = 0.0;
			element(1,3) = 0.0;

			element(2,0) = 0.0;
			element(2,1) = 0.0;
			element(2,2) = 1.0;
			element(2,3) = 0.0;

			element(3,0) = 0.0; 
			element(3,1) = 0.0; 
			element(3,2) = 0.0;
			element(3,3) = 1.0;
		}


		static matrix4 identity()
		{
			static matrix4 mident (
				1.0, 0.0, 0.0, 0.0,
				0.0, 1.0, 0.0, 0.0,
				0.0, 0.0, 1.0, 0.0,
				0.0, 0.0, 0.0, 1.0  );
			return mident;
		}


		void set_scale( real s )
		{
			element(0,0) = s;
			element(1,1) = s;
			element(2,2) = s;
		}

		void set_scale( const vec3 & s )
		{
			element(0,0) = s.v[0];
			element(1,1) = s.v[1];
			element(2,2) = s.v[2];
		}


		void set_translate( const vec3 & t )
		{
			element(0,3) = t.v[0];
			element(1,3) = t.v[1];
			element(2,3) = t.v[2];
		}

		void set_row(int r, const vec4 & t)
		{
			element(r,0) = t.v[0];
			element(r,1) = t.v[1];
			element(r,2) = t.v[2];
			element(r,3) = t.v[3];
		}

		void set_column(int c, const vec4 & t)
		{
			element(0,c) = t.v[0];
			element(1,c) = t.v[1];
			element(2,c) = t.v[2];
			element(3,c) = t.v[3];
		}


		void get_row(int r, vec4 & t) const
		{
			t.v[0] = element(r,0);
			t.v[1] = element(r,1);
			t.v[2] = element(r,2);
			t.v[3] = element(r,3);
		}

		vec4 get_row(int r) const
		{
			vec4 v; get_row(r, v);
			return v;
		}

		void get_column(int c, vec4 & t) const
		{
			t.v[0] = element(0,c);
			t.v[1] = element(1,c);
			t.v[2] = element(2,c);
			t.v[3] = element(3,c);
		}

		vec4 get_column(int c) const
		{
			vec4 v; get_column(c, v);
			return v;
		}

		matrix4 inverse() const
		{
			matrix4 minv;

			real r1[8], r2[8], r3[8], r4[8];
			real *s[4], *tmprow;

			s[0] = &r1[0];
			s[1] = &r2[0];
			s[2] = &r3[0];
			s[3] = &r4[0];

			register int i,j,p,jj;
			for(i=0;i<4;i++)
			{
				for(j=0;j<4;j++)
				{
					s[i][j] = element(i,j);
					if(i==j) s[i][j+4] = 1.0;
					else     s[i][j+4] = 0.0;
				}
			}
			real scp[4];
			for(i=0;i<4;i++)
			{
				scp[i] = real(fabs(s[i][0]));
				for(j=1;j<4;j++)
					if(real(fabs(s[i][j])) > scp[i]) scp[i] = real(fabs(s[i][j]));
				if(scp[i] == 0.0) return minv; // singular matrix!
			}

			int pivot_to;
			real scp_max;
			for(i=0;i<4;i++)
			{
				// select pivot row
				pivot_to = i;
				scp_max = real(fabs(s[i][i]/scp[i]));
				// find out which row should be on top
				for(p=i+1;p<4;p++)
					if(real(fabs(s[p][i]/scp[p])) > scp_max)
					{ scp_max = real(fabs(s[p][i]/scp[p])); pivot_to = p; }
					// Pivot if necessary
					if(pivot_to != i)
					{
						tmprow = s[i];
						s[i] = s[pivot_to];
						s[pivot_to] = tmprow;
						real tmpscp;
						tmpscp = scp[i];
						scp[i] = scp[pivot_to];
						scp[pivot_to] = tmpscp;
					}

					real mji;
					// perform gaussian elimination
					for(j=i+1;j<4;j++)
					{
						mji = s[j][i]/s[i][i];
						s[j][i] = 0.0;
						for(jj=i+1;jj<8;jj++)
							s[j][jj] -= mji*s[i][jj];
					}
			}
			if(s[3][3] == 0.0) return minv; // singular matrix!

			//
			// Now we have an upper triangular matrix.
			//
			//  x x x x | y y y y
			//  0 x x x | y y y y 
			//  0 0 x x | y y y y
			//  0 0 0 x | y y y y
			//
			//  we'll back substitute to get the inverse
			//
			//  1 0 0 0 | z z z z
			//  0 1 0 0 | z z z z
			//  0 0 1 0 | z z z z
			//  0 0 0 1 | z z z z 
			//

			real mij;
			for(i=3;i>0;i--)
			{
				for(j=i-1;j > -1; j--)
				{
					mij = s[j][i]/s[i][i];
					for(jj=j+1;jj<8;jj++)
						s[j][jj] -= mij*s[i][jj];
				}
			}

			for(i=0;i<4;i++)
				for(j=0;j<4;j++)
					minv(i,j) = s[i][j+4] / s[i][i];

			return minv;
		}


		matrix4 transpose() const
		{
			matrix4 mtrans;

			for(int i=0;i<4;i++)
				for(int j=0;j<4;j++)
					mtrans(i,j) = element(j,i);		
			return mtrans;
		}

		matrix4 & mult_right( const matrix4 & b )
		{
			matrix4 mt(*this);
			set_value(real(0));

			for(int i=0; i < 4; i++)
				for(int j=0; j < 4; j++)
					for(int c=0; c < 4; c++)
						element(i,j) += mt(i,c) * b(c,j);
			return *this;
		}    

		matrix4 & mult_left( const matrix4 & b )
		{
			matrix4 mt(*this);
			set_value(real(0));

			for(int i=0; i < 4; i++)
				for(int j=0; j < 4; j++)
					for(int c=0; c < 4; c++)
						element(i,j) += b(i,c) * mt(c,j);
			return *this;
		}

		// dst = M * src
		void mult_matrix_vec( const vec3 &src, vec3 &dst ) const
		{
			real w = (
				src.v[0] * element(3,0) +
				src.v[1] * element(3,1) + 
				src.v[2] * element(3,2) +
				element(3,3)          );

			assert(w != 0.0f);

			dst.v[0]  = (
				src.v[0] * element(0,0) +
				src.v[1] * element(0,1) +
				src.v[2] * element(0,2) +
				element(0,3)          ) / w;
			dst.v[1]  = (
				src.v[0] * element(1,0) +
				src.v[1] * element(1,1) +
				src.v[2] * element(1,2) +
				element(1,3)          ) / w;
			dst.v[2]  = (
				src.v[0] * element(2,0) +
				src.v[1] * element(2,1) + 
				src.v[2] * element(2,2) +
				element(2,3)          ) / w;
		}

		void mult_matrix_vec( vec3 & src_and_dst) const
		{ mult_matrix_vec(vec3(src_and_dst), src_and_dst); }


		// dst = src * M
		void mult_vec_matrix( const vec3 &src, vec3 &dst ) const
		{
			real w = (
				src.v[0] * element(0,3) +
				src.v[1] * element(1,3) +
				src.v[2] * element(2,3) +
				element(3,3)          );

			assert(w != 0.0f);

			dst.v[0]  = (
				src.v[0] * element(0,0) +
				src.v[1] * element(1,0) + 
				src.v[2] * element(2,0) + 
				element(3,0)          ) / w;
			dst.v[1]  = (
				src.v[0] * element(0,1) +
				src.v[1] * element(1,1) +
				src.v[2] * element(2,1) +
				element(3,1)          ) / w;
			dst.v[2]  = (
				src.v[0] * element(0,2) +
				src.v[1] * element(1,2) +
				src.v[2] * element(2,2) +
				element(3,2)          ) / w;
		}


		void mult_vec_matrix( vec3 & src_and_dst) const
		{ mult_vec_matrix(vec3(src_and_dst), src_and_dst); }

		// dst = M * src
		void mult_matrix_vec( const vec4 &src, vec4 &dst ) const
		{
			dst.v[0]  = (
				src.v[0] * element(0,0) +
				src.v[1] * element(0,1) +
				src.v[2] * element(0,2) +
				src.v[3] * element(0,3));
			dst.v[1]  = (
				src.v[0] * element(1,0) +
				src.v[1] * element(1,1) +
				src.v[2] * element(1,2) +
				src.v[3] * element(1,3));
			dst.v[2]  = (
				src.v[0] * element(2,0) +
				src.v[1] * element(2,1) + 
				src.v[2] * element(2,2) +
				src.v[3] * element(2,3));
			dst.v[3] = (
				src.v[0] * element(3,0) +
				src.v[1] * element(3,1) + 
				src.v[2] * element(3,2) +
				src.v[3] * element(3,3));
		}

		void mult_matrix_vec( vec4 & src_and_dst) const
		{ mult_matrix_vec(vec4(src_and_dst), src_and_dst); }


		// dst = src * M
		void mult_vec_matrix( const vec4 &src, vec4 &dst ) const
		{
			dst.v[0]  = (
				src.v[0] * element(0,0) +
				src.v[1] * element(1,0) + 
				src.v[2] * element(2,0) + 
				src.v[3] * element(3,0));
			dst.v[1]  = (
				src.v[0] * element(0,1) +
				src.v[1] * element(1,1) +
				src.v[2] * element(2,1) +
				src.v[3] * element(3,1));
			dst.v[2]  = (
				src.v[0] * element(0,2) +
				src.v[1] * element(1,2) +
				src.v[2] * element(2,2) +
				src.v[3] * element(3,2));
			dst.v[3] = (
				src.v[0] * element(0,3) +
				src.v[1] * element(1,3) +
				src.v[2] * element(2,3) +
				src.v[3] * element(3,3));
		}


		void mult_vec_matrix( vec4 & src_and_dst) const
		{ mult_vec_matrix(vec4(src_and_dst), src_and_dst); }


		// dst = M * src
		void mult_matrix_dir( const vec3 &src, vec3 &dst ) const
		{
			dst.v[0]  = (
				src.v[0] * element(0,0) +
				src.v[1] * element(0,1) +
				src.v[2] * element(0,2) ) ;
			dst.v[1]  = ( 
				src.v[0] * element(1,0) +
				src.v[1] * element(1,1) +
				src.v[2] * element(1,2) ) ;
			dst.v[2]  = ( 
				src.v[0] * element(2,0) +
				src.v[1] * element(2,1) + 
				src.v[2] * element(2,2) ) ;
		}


		void mult_matrix_dir( vec3 & src_and_dst) const
		{ mult_matrix_dir(vec3(src_and_dst), src_and_dst); }


		// dst = src * M
		void mult_dir_matrix( const vec3 &src, vec3 &dst ) const
		{
			dst.v[0]  = ( 
				src.v[0] * element(0,0) +
				src.v[1] * element(1,0) +
				src.v[2] * element(2,0) ) ;
			dst.v[1]  = ( 
				src.v[0] * element(0,1) +
				src.v[1] * element(1,1) +
				src.v[2] * element(2,1) ) ;
			dst.v[2]  = (
				src.v[0] * element(0,2) +
				src.v[1] * element(1,2) + 
				src.v[2] * element(2,2) ) ;
		}


		void mult_dir_matrix( vec3 & src_and_dst) const
		{ mult_dir_matrix(vec3(src_and_dst), src_and_dst); }


		real & operator () (int row, int col)
		{ return element(row,col); }

		const real & operator () (int row, int col) const
		{ return element(row,col); }

		real & element (int row, int col)
		{ return m[row | (col<<2)]; }

		const real & element (int row, int col) const
		{ return m[row | (col<<2)]; }

		matrix4 & operator *= ( const matrix4 & mat )
		{
			mult_right( mat );
			return *this;
		}

		matrix4 & operator *= ( const real & r )
		{
			for (int i = 0; i < 4; ++i)
			{
				element(0,i) *= r;
				element(1,i) *= r;
				element(2,i) *= r;
				element(3,i) *= r;
			}
			return *this;
		}

		matrix4 & operator += ( const matrix4 & mat )
		{
			for (int i = 0; i < 4; ++i)
			{
				element(0,i) += mat.element(0,i);
				element(1,i) += mat.element(1,i);
				element(2,i) += mat.element(2,i);
				element(3,i) += mat.element(3,i);
			}
			return *this;
		}

		friend matrix4 operator * ( const matrix4 & m1,	const matrix4 & m2 );
		friend bool operator == ( const matrix4 & m1, const matrix4 & m2 );
		friend bool operator != ( const matrix4 & m1, const matrix4 & m2 );

		//protected:
		real m[16];
	};

	inline  
		matrix4 operator * ( const matrix4 & m1, const matrix4 & m2 )
	{
		matrix4 product;

		product = m1;
		product.mult_right(m2);

		return product;
	}

	inline
		bool operator ==( const matrix4 &m1, const matrix4 &m2 )
	{
		return ( 
			m1(0,0) == m2(0,0) &&
			m1(0,1) == m2(0,1) &&
			m1(0,2) == m2(0,2) &&
			m1(0,3) == m2(0,3) &&
			m1(1,0) == m2(1,0) &&
			m1(1,1) == m2(1,1) &&
			m1(1,2) == m2(1,2) &&
			m1(1,3) == m2(1,3) &&
			m1(2,0) == m2(2,0) &&
			m1(2,1) == m2(2,1) &&
			m1(2,2) == m2(2,2) &&
			m1(2,3) == m2(2,3) &&
			m1(3,0) == m2(3,0) &&
			m1(3,1) == m2(3,1) &&
			m1(3,2) == m2(3,2) &&
			m1(3,3) == m2(3,3) );
	}

	inline
		bool operator != ( const matrix4 & m1, const matrix4 & m2 )
	{ return !( m1 == m2 ); }  

	class quaternion
	{
	public:

		quaternion()
		{
			*this = identity();
		}

		quaternion( const real v[4] )
		{
			set_value( v );
		}


		quaternion( real q0, real q1, real q2, real q3 )
		{
			set_value( q0, q1, q2, q3 );
		}


		quaternion( const matrix4 & m )
		{
			set_value( m );
		}


		quaternion( const vec3 &axis, real radians )
		{
			set_value( axis, radians );
		}


		quaternion( const vec3 &rotateFrom, const vec3 &rotateTo )
		{
			set_value( rotateFrom, rotateTo );
		}

		quaternion( const vec3 & from_look, const vec3 & from_up,
			const vec3 & to_look, const vec3& to_up)
		{
			set_value(from_look, from_up, to_look, to_up);
		}

		const real * get_value() const
		{
			return  &q[0];
		}

		void get_value( real &q0, real &q1, real &q2, real &q3 ) const
		{
			q0 = q[0];
			q1 = q[1];
			q2 = q[2];
			q3 = q[3];
		}

		quaternion & set_value( real q0, real q1, real q2, real q3 )
		{
			q[0] = q0;
			q[1] = q1;
			q[2] = q2;
			q[3] = q3;
			counter = 0;
			return *this;
		}

		void get_value( vec3 &axis, real &radians ) const
		{
			radians = real(acos( q[3] ) * 2.0f);
			if ( radians == 0.0f )
				axis = vec3( 0.0, 0.0, 1.0 );
			else
			{
				axis.v[0] = q[0];
				axis.v[1] = q[1];
				axis.v[2] = q[2];
				axis.normalize();
			}
		}

		void get_value( matrix4 & m ) const
		{
			real s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;

			real norm = q[0] * q[0] + q[1] * q[1] + q[2] * q[2] + q[3] * q[3];

			s = (equivalent(norm,0.0f)) ? 0.0f : ( 2.0f / norm );

			xs = q[0] * s;
			ys = q[1] * s;
			zs = q[2] * s;

			wx = q[3] * xs;
			wy = q[3] * ys;
			wz = q[3] * zs;

			xx = q[0] * xs;
			xy = q[0] * ys;
			xz = q[0] * zs;

			yy = q[1] * ys;
			yz = q[1] * zs;
			zz = q[2] * zs;

			m(0,0) = real( 1.0f - ( yy + zz ));
			m(1,0) = real ( xy + wz );
			m(2,0) = real ( xz - wy );

			m(0,1) = real ( xy - wz );
			m(1,1) = real ( 1.0f - ( xx + zz ));
			m(2,1) = real ( yz + wx );

			m(0,2) = real ( xz + wy );
			m(1,2) = real ( yz - wx );
			m(2,2) = real ( 1.0f - ( xx + yy ));

			m(3,0) = m(3,1) = m(3,2) = m(0,3) = m(1,3) = m(2,3) = 0.0f;
			m(3,3) = 1.0f;
		}

		quaternion & set_value( const real * qp )
		{
			memcpy(q,qp,sizeof(real) * 4);

			counter = 0;
			return *this;
		}

		quaternion & set_value( const matrix4 & m )
		{
			real tr, s;
			int i, j, k;
			const int nxt[3] = { 1, 2, 0 };

			tr = m(0,0) + m(1,1) + m(2,2);

			if ( tr > 0.0f )
			{
				s = real(sqrt( tr + m(3,3) ));
				q[3] = real ( s * 0.5 );
				s = real(0.5) / s;

				q[0] = real ( ( m(1,2) - m(2,1) ) * s );
				q[1] = real ( ( m(2,0) - m(0,2) ) * s );
				q[2] = real ( ( m(0,1) - m(1,0) ) * s );
			}
			else
			{
				i = 0;
				if ( m(1,1) > m(0,0) )
					i = 1;

				if ( m(2,2) > m(i,i) )
					i = 2;

				j = nxt[i];
				k = nxt[j];

				s = real(sqrt( ( m(i,j) - ( m(j,j) + m(k,k) )) + 1.0f ));

				q[i] = real ( s * 0.5 );
				s = real(0.5 / s);

				q[3] = real ( ( m(j,k) - m(k,j) ) * s );
				q[j] = real ( ( m(i,j) + m(j,i) ) * s );
				q[k] = real ( ( m(i,k) + m(k,i) ) * s );
			}

			counter = 0;
			return *this;
		}

		quaternion & set_value( const vec3 &axis, real theta )
		{
			real sqnorm = axis.square_norm();

			if (sqnorm <= EPSILON)
			{
				// axis too small.
				x = y = z = 0.0;
				w = 1.0;
			} 
			else 
			{
				theta *= real(0.5);
				real sin_theta = real(sin(theta));

				if (!equivalent(sqnorm,1.0f)) 
					sin_theta /= real(sqrt(sqnorm));
				x = sin_theta * axis.v[0];
				y = sin_theta * axis.v[1];
				z = sin_theta * axis.v[2];
				w = real(cos(theta));
			}
			return *this;
		}

		quaternion & set_value( const vec3 & rotateFrom, const vec3 & rotateTo )
		{
			vec3 p1, p2;
			real alpha;

			p1 = rotateFrom; 
			p1.normalize();
			p2 = rotateTo;  
			p2.normalize();

			alpha = p1.dot(p2);

			if(equivalent(alpha,1.0f))
			{ 
				*this = identity(); 
				return *this; 
			}

			// ensures that the anti-parallel case leads to a positive dot
			if(equivalent(alpha,-1.0f))
			{
				vec3 v;

				if(p1.v[0] != p1.v[1] || p1.v[0] != p1.v[2])
					v = vec3(p1.v[1], p1.v[2], p1.v[0]);
				else
					v = vec3(-p1.v[0], p1.v[1], p1.v[2]);

				v -= p1 * p1.dot(v);
				v.normalize();

				set_value(v, PI);
				return *this;
			}

			p1 = p1.cross(p2);  
			p1.normalize();
			set_value(p1,real(acos(alpha)));

			counter = 0;
			return *this;
		}

		quaternion & set_value( const vec3 & from_look, const vec3 & from_up,
			const vec3 & to_look, const vec3 & to_up)
		{
			quaternion r_look = quaternion(from_look, to_look);

			vec3 rotated_from_up(from_up);
			r_look.mult_vec(rotated_from_up);

			quaternion r_twist = quaternion(rotated_from_up, to_up);

			*this = r_twist;
			*this *= r_look;
			return *this;
		}

		quaternion & operator *= ( const quaternion & qr )
		{
			quaternion ql(*this);

			w = ql.w * qr.w - ql.x * qr.x - ql.y * qr.y - ql.z * qr.z;
			x = ql.w * qr.x + ql.x * qr.w + ql.y * qr.z - ql.z * qr.y;
			y = ql.w * qr.y + ql.y * qr.w + ql.z * qr.x - ql.x * qr.z;
			z = ql.w * qr.z + ql.z * qr.w + ql.x * qr.y - ql.y * qr.x;

			counter += qr.counter;
			counter++;
			counter_normalize();
			return *this;
		}

		void normalize()
		{
			real rnorm = 1.0f / real(sqrt(w * w + x * x + y * y + z * z));
			if (equivalent(rnorm, 0.0f))
				return;
			x *= rnorm;
			y *= rnorm;
			z *= rnorm;
			w *= rnorm;
			counter = 0;
		}

		friend bool operator == ( const quaternion & q1, const quaternion & q2 );      

		friend bool operator != ( const quaternion & q1, const quaternion & q2 );

		friend quaternion operator * ( const quaternion & q1, const quaternion & q2 );

		bool equals( const quaternion & r, real tolerance ) const
		{
			real t;

			t = (
				(q[0]-r.q[0])*(q[0]-r.q[0]) +
				(q[1]-r.q[1])*(q[1]-r.q[1]) +
				(q[2]-r.q[2])*(q[2]-r.q[2]) +
				(q[3]-r.q[3])*(q[3]-r.q[3]) );
			if(t > EPSILON) 
				return false;
			return 1;
		}

		quaternion & conjugate()
		{
			q[0] *= -1.0f;
			q[1] *= -1.0f;
			q[2] *= -1.0f;
			return *this;
		}

		quaternion & invert()
		{
			return conjugate();
		}

		quaternion inverse() const
		{
			quaternion r = *this;
			return r.invert();
		}

		//
		// Quaternion multiplication with cartesian vector
		// v' = q*v*q(star)
		//
		void mult_vec( const vec3 &src, vec3 &dst ) const
		{
			real v_coef = w * w - x * x - y * y - z * z;                     
			real u_coef = 2.0f * (src.v[0] * x + src.v[1] * y + src.v[2] * z);  
			real c_coef = 2.0f * w;                                       

			dst.v[0] = v_coef * src.v[0] + u_coef * x + c_coef * (y * src.v[2] - z * src.v[1]);
			dst.v[1] = v_coef * src.v[1] + u_coef * y + c_coef * (z * src.v[0] - x * src.v[2]);
			dst.v[2] = v_coef * src.v[2] + u_coef * z + c_coef * (x * src.v[1] - y * src.v[0]);
		}

		void mult_vec( vec3 & src_and_dst) const
		{
			mult_vec(vec3(src_and_dst), src_and_dst);
		}

		void scale_angle( real scaleFactor )
		{
			vec3 axis;
			real radians;

			get_value(axis, radians);
			radians *= scaleFactor;
			set_value(axis, radians);
		}

		static quaternion slerp( const quaternion & p, const quaternion & q, real alpha )
		{
			quaternion r;

			real cos_omega = p.x * q.x + p.y * q.y + p.z * q.z + p.w * q.w;
			// if B is on opposite hemisphere from A, use -B instead

			int bflip;
			if ( ( bflip = (cos_omega < 0.0f)) )
				cos_omega = -cos_omega;

			// complementary interpolation parameter
			real beta = 1.0f - alpha;     

			if(cos_omega >= 1.0f - EPSILON)
				return p;

			real omega = real(acos(cos_omega));
			real one_over_sin_omega = 1.0f / real(sin(omega));

			beta    = real(sin(omega*beta)  * one_over_sin_omega);
			alpha   = real(sin(omega*alpha) * one_over_sin_omega);

			if (bflip)
				alpha = -alpha;

			r.x = beta * p.q[0]+ alpha * q.q[0];
			r.y = beta * p.q[1]+ alpha * q.q[1];
			r.z = beta * p.q[2]+ alpha * q.q[2];
			r.w = beta * p.q[3]+ alpha * q.q[3];
			return r;
		}

		static quaternion identity()
		{
			static quaternion ident( vec3( 0.0, 0.0, 0.0 ), 1.0f );
			return ident;
		}

		real & operator []( int i )
		{
			assert(i < 4);
			return q[i];
		}

		const real & operator []( int i ) const
		{
			assert(i < 4);
			return q[i];
		}

	protected:

		void counter_normalize()
		{
			if (counter > QUATERNION_NORMALIZATION_THRESHOLD)
				normalize();
		}

		union 
		{
			struct 
			{
				real q[4];
			};
			struct 
			{
				real x;
				real y;
				real z;
				real w;
			};
		};

		// renormalization counter
		unsigned char counter;
	};

	inline
		bool operator == ( const quaternion & q1, const quaternion & q2 )
	{
		return (equivalent(q1.x, q2.x) &&
			equivalent(q1.y, q2.y) &&
			equivalent(q1.z, q2.z) &&
			equivalent(q1.w, q2.w) );
	}

	inline
		bool operator != ( const quaternion & q1, const quaternion & q2 )
	{ 
		return ! ( q1 == q2 ); 
	}

	inline
		quaternion operator * ( const quaternion & q1, const quaternion & q2 )
	{	
		quaternion r(q1); 
		r *= q2; 
		return r; 
	}

  typedef vec3 vec3f;
  typedef vec4 vec4f;
  typedef quaternion quaternionf;
  typedef matrix4 matrix4f;
}

#endif