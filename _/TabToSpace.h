#pragma once



/**@ingroup STRING
 * �^�u�R�[�h���X�y�[�X�ɕϊ����܂��B<BR>
 * ���s�͊܂܂Ȃ��悤�ɂ��Ă��������B
 * @param pstrText ���̃e�L�X�g�B
 * @param iTabStep �^�u�X�e�b�v���B�ȗ�����ƂS�ɂȂ�܂��B
 * @return �^�O�R�[�h�̊܂܂Ȃ�������B
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

