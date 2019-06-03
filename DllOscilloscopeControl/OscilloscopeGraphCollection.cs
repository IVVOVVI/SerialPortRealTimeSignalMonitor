using System;
using System.Collections;
using System.Collections.Generic;

namespace Oscilloscope
{

	///<summary>示波器图形集合（所有项都不为null）</summary>
	public class OscilloscopeGraphCollection : IList, IEnumerable<OscilloscopeGraph>
	{

		///<summary>图形集合</summary>
		private List<OscilloscopeGraph> m_Graphs = new List<OscilloscopeGraph>();

		///<summary>枚举器</summary>
		public IEnumerator<OscilloscopeGraph> GetEnumerator()
		{
			return m_Graphs.GetEnumerator();
		}

		///<summary>是否只读</summary>
		public bool IsReadOnly { get; internal set; } = false;

		///<summary>索引位置读写</summary>
		public object this[int index]
		{
			get { return m_Graphs[index]; }
			set
			{
				if (IsReadOnly) throw new InvalidOperationException();
				var item = value as OscilloscopeGraph;
				if (item != null) m_Graphs[index] = item;
			}
		}

		///<summary>是否是固定大小（固定值：否）</summary>
		public bool IsFixedSize { get { return false; } }

		///<summary>项数</summary>
		public int Count { get { return m_Graphs.Count; } }

		///<summary>获取同步访问对象</summary>
		public object SyncRoot { get { return ((ICollection)m_Graphs).SyncRoot; } }

		///<summary>是否同步访问</summary>
		public bool IsSynchronized { get { return ((ICollection)m_Graphs).IsSynchronized; } }

		///<summary>添加</summary>
		public int Add(object value)
		{
			if (IsReadOnly) throw new InvalidOperationException();
			var item = value as OscilloscopeGraph;
			if (item != null)
			{
				var index = m_Graphs.Count;
				m_Graphs.Add(item);
				return index;
			}
			return -1;
		}

		///<summary>清除</summary>
		public void Clear()
		{
			if (IsReadOnly) throw new InvalidOperationException();
			m_Graphs.Clear();
		}

		///<summary>是否包含指定的项</summary>
		public bool Contains(object value)
		{
			var item = value as OscilloscopeGraph;
			if (item == null) return false;
			return m_Graphs.Contains(item);
		}

		///<summary>复制到</summary>
		public void CopyTo(Array array, int index)
		{
			var temp = array as OscilloscopeGraph[];
			if (temp != null) m_Graphs.CopyTo(temp, index);
		}

		///<summary>查找</summary>
		public int IndexOf(object value)
		{
			var item = value as OscilloscopeGraph;
			if (item == null) return -1;
			return m_Graphs.IndexOf(item);
		}

		///<summary>插入</summary>
		public void Insert(int index, object value)
		{
			if (IsReadOnly) throw new InvalidOperationException();
			var item = value as OscilloscopeGraph;
			if (item != null) m_Graphs.Insert(index, item);
		}

		///<summary>移除指定的项</summary>
		public void Remove(object value)
		{
			if (IsReadOnly) throw new InvalidOperationException();
			var item = value as OscilloscopeGraph;
			if (item != null) m_Graphs.Remove(item);
		}

		///<summary>移除指定位置上的项</summary>
		public void RemoveAt(int index)
		{
			if (IsReadOnly) throw new InvalidOperationException();
			m_Graphs.RemoveAt(index);
		}

		///<summary>枚举器</summary>
		IEnumerator IEnumerable.GetEnumerator()
		{
			return m_Graphs.GetEnumerator();
		}

	}

}