#pragma once



/**@ingroup STRING
 * タブコードをスペースに変換します。<BR>
 * 改行は含まないようにしてください。
 * @param pstrText 元のテキスト。
 * @param iTabStep タブステップ数。省略すると４になります。
 * @return タグコードの含まない文字列。
 */
inline CStr TabToSpace(LPCTSTR lpszText, size_t step = 4)
{
	CStr strWork;
	if ( step >= 1 && step <= 8 )
	{
		strWork = lpszText;
		while ( true )
		{
			int pos = strWork.Find('\t');
			if ( pos < 0 )
			{
				break;
			}
			CStr str = strWork;
			const LPCTSTR alpszSpace[8] =
			{
				NULL,			_T(" "),		_T("   "),		_T("  "),
				_T("    "),		_T("     "),	_T("      "),	_T("       "),
			};
			strWork = str.Left(pos) + alpszSpace[step - (pos % step)] + str.Mid(pos + 1);
		}
	}
	return strWork;
}

