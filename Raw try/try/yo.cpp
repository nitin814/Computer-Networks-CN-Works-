#include <bits/stdc++.h>
using namespace std;
#define ll long long

int main ()
{
    ll t; cin>>t;
    while (t--)
    {
        ll n; cin>>n;
        vector<ll> cnt(n+1 , 0); ll k;
        for (ll i=0;i<n;i++)
        {
            cin>>k;
            cnt[k]++;
        }

        vector<ll> pairs_gcd(n+1 , 0); // total number of pairs having their gcd as i

        for (ll i = n;i>0;i--)
        {   
            ll count1 = 0; ll count2 = 0;
            for (ll j = i;j<=n;j+=i)
            {
                count1 += cnt[j];
                count2 += pairs_gcd[j];
            }

            pairs_gcd[i] = (count1 * (count1 - 1)) / 2 - count2;
        }

        ll ans = (n*(n-1))/2;
        for (ll i = 1;i<=n;i++)
        {
            if (cnt[i]>0)
            {
                for (ll j = i;j<=n;j+=i)
                {
                    ans -= pairs_gcd[j];
                    pairs_gcd[j] = 0;
                    cnt[j] = 0;
                }
            }
        }

        cout<<ans<<endl;
    }
}